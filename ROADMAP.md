# Roadmap

## Project Status

For now, we can display each ways and nodes that belong to a relation
which itself represents a bus line.

## Next steps

The next step would be to :

- find the right ordering of nodes (bus stops) composing a bus line.
  In fact, we have to find out if the list of nodes composing a path is ordered
- compute distance between each node
  an approach based on putting a rectangle between 2 nodes is proposed
- find nearest bus stop to a the start point
- apply algorithm to find the shortest, cheapest path

## Algorithm for graph building

- start from the first point
- consider near point to be the same
- find the nearest bus stop
- find path between them
- compute path length and sum
- iterate

## Data cleaning and enhancing features

- group near point manually
- use the one with a name as reference and the other as reference in the path
- compute distance between each node
- restore path using driver API
- rename bus stop
- plot other data like bus stop from another source that are near the path
