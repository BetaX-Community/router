/***************************************************************************
 *  Project:    betax-router
 *  File:       overpass-api.h
 *  Language:   C++
 *
 *  Author:     Andrianambinina Marius Rabenarivo 
 *  E-Mail:     marius@betax.mg 
 *
 *  Description:    Download OSM XML file from the overpass API
 *
 *  Dependencies:       libboost
 *
 ***************************************************************************/

#include <curl/curl.h>
#include <boost/program_options.hpp>

#include <iostream>
#include <fstream>
#include <string>

#ifndef OVERPASS_API_H
#define OVERPASS_API_H

class CURLplusplus
{
    private:
        CURL* curl;
        std::stringstream ss;
        long http_code;
    public:
        CURLplusplus()
            : curl(curl_easy_init())
              , http_code(0)
        {
        }
        ~CURLplusplus()
        {
            if (curl) curl_easy_cleanup(curl);
        }
        std::string Get(const std::string& url)
        {
            CURLcode res;
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);

            ss.str("");
            http_code = 0;
            res = curl_easy_perform(curl);
            if (res != CURLE_OK)
            {
                throw std::runtime_error(curl_easy_strerror(res));
            }
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
            return ss.str();
        }
        long GetHttpCode()
        {
            return http_code;
        }
    private:
        static size_t write_data(void *buffer, size_t size, size_t nmemb, void *userp)
        {
            return static_cast<CURLplusplus*>(userp)->Write(buffer,size,nmemb);
        }
        size_t Write(void *buffer, size_t size, size_t nmemb)
        {
            ss.write((const char*)buffer,size*nmemb);
            return size*nmemb;
        }
};

class Xml 
{
    /*
     * Downloads OSM data from the overpass API
     */
    private:
        const std::string _file;
        const float _latmin, _lonmin, _latmax, _lonmax;

    public:
        std::string tempstr;

    Xml (std::string file, float lonmin, float latmin, float lonmax, float latmax)
        : _file (file), _lonmin (lonmin), _latmin (latmin),
                        _lonmax (lonmax), _latmax (latmax)
    {
      std::cout << "Downloading overpass query ... ";
      std::cout.flush ();
      tempstr = readOverpass ();
      // Write raw xml data to _file:
      std::ofstream out_file;
      out_file.open (_file.c_str (), std::ofstream::out);
      out_file << tempstr;
      out_file.flush ();
      out_file.close ();
      std::cout << " stored in " << _file << std::endl;
    }

    std::string get_file () { return _file; }
    float get_lonmin () { return _lonmin;   }
    float get_latmin () { return _latmin;   }
    float get_lonmax () { return _lonmax;   }
    float get_latmax () { return _latmax;   }

    std::string readOverpass ();
};

std::string Xml::readOverpass()
{
    const std::string key = "[\"route\"=\"bus\"]",
                url_base = "http://overpass-api.de/api/interpreter?data=";
    std::stringstream bbox, query, url;

    bbox << "";
    bbox << "(" << get_latmin () << "," << get_lonmin () << "," << 
        get_latmax () << "," << get_lonmax () << ")";

    query << "";
    query << "(rel" << key << bbox.str() << ";>;);out meta;";
    std::string escaped_query;
    CURL *curl = curl_easy_init();
    if(curl) {
      char *output = curl_easy_escape(curl, query.str().c_str(), query.str().size());
      if(output) {
	std::string tmp_s(output);
	escaped_query = tmp_s;
	curl_free(output);
      }
    }
    url << "";
    url << url_base << escaped_query;
    std::cout << url.str() << std::endl;
    CURLplusplus client;
    std::string x = client.Get (url.str().c_str ());
    
    return x;
};

#endif
