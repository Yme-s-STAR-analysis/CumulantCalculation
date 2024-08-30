# Cumulant Calculation

Version: 6.0

Author: Yige Huang

## Usage

1. `make`

2. `./runCumulant mTerms cent_edge.txt w8.txt`

## Patch Note

30.08.2024 by yghuang (v6.0):

1. Combined NpartLoader and CentDefinition
    * The centrality splitting is changed into left-close and right-open

2. Temporarily disable `duoCBWC`

3. Bug fix: the `nEvent` was `int` but it was supposed to be `double`, since we would multiple the reweight factor

03.06.2024 by yghuang (v5.0.3):

1. Update: Npart loader, centrality reader, reweight technique in cbwc

2. Completed version

> Completed version

18.08.2023 by yghuang (v3.0):

1. Full constructed cumulant calculation system
