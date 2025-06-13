# Cumulant Calculation

Version: 7.1

Author: Yige Huang

## Usage

1. `make`

2. `./runCumulant mTerms cent_edge.txt w8.txt`

## Patch Note

13.06.2025 by yghuang (v7.1):

1. Support homogeneous factorail cumulant check, which only calculate proton and antiproton factorial cumulant, and normalized by k1 or k1 to nth power
    * Note that, in order to function this, HOMO3 option must turn on in GetTerms

07.10.2024 by yghuang (v7.0):

1. For CBWC on ratios, there are 2 methods:
    1. Directly apply CBWC on ratios
    2. Take ratios on CBWCorrected cumulants

    * Both of them are calculated and stored now
    * Note that, previous R42 (with method2) is now named as R42r, and the new R42 comes from method1

2. Some cosmetic updates on codes

03.10.2024 by yghuang (v6.2):

1. Fix a fatal bug: the error for kappa calculation was incorrect for forth order case
    * The bin entries was not divided correctly, and only the errors of kappa are affected, the ratios were normal

16.09.2024 by yghuang (v6.1):

1. Add a efficient version that only deals utmost 4th order
    * The mTerms should come from corresponding GetTerms of utmost 4th order version as well

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
