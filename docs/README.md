# Overview

Kronos is a component-driven framework inspired by the [Fprime](https://github.com/nasa/fprime) architecture. The
framework splits up a complex system into various components. A component is defined as an isolated collection of logic
created to achieve a single specific goal. Components communicate between each other through the use of busses by
publishing or listening to events carried through them. Each bus may only carry one event type and is used to
directionally link various components together.