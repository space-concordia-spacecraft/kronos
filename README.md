{#mainpage} Kronos Framework
============

This framework was originally developed by [Space Concordia](http://www.spaceconcordia.ca/en/) members to be used in [SC-ODIN](http://www.spaceconcordia.ca/en/division/spacecraft/).

## Overview

Kronos is a component-driven framework inspired by the [Fprime](https://github.com/nasa/fprime) architecture. The framework splits up a complex system into various components. A component is defined as an isolated collection of logic created to achieve a single specific goal. Components communicate between each other through the use of busses by publishing or listening to events carried through them. Each bus may only carry one event type and is used to directionally link various components together.

More detail about each part about the building blocks of the architecture are explained in [this README](https://github.com/space-concordia-spacecraft/kronos/blob/main/lib/README.md). To read more about the testing library used to unit test all the components read [this README](https://github.com/space-concordia-spacecraft/kronos/blob/main/test"/README.md).

The framework is currently built around FreeRTOS and Reliance Edge but, as a future endeavour, it will be modulated to allow for support of other operating systems and file systems.

To install and set up the project please refer to the [installation guide](https://github.com/space-concordia-spacecraft/kronos/blob/main/lib/INSTALL.md).