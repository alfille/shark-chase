# The Shark Puzzle

![picture](picture.png)

This is a simple puzzzle but interesting to explore different solution techniques.

## The rules:

* You are in the center of a __circular island__. 🏝️
* There is a __shark__ that wants to eat you, &#129416;
  * The shark can swim 4 times as fast as you can run.
* You can see each other.
* If you get to the shore before the shark, you can escape . (Don't ask how this works &#128533;)

## Goal

* Make it to a point on the shore before the shark arrives
* Find the shortest safe route

## Make it simple

* We'll call the __radius__ of the island __1 km__
  * The __circumference__ is thus __2 &pi; km__
* You can run at __1 km/hr__ &#127939;
  * So the shark swims at __4 km/hr__

## Why it's hard

If you run straight away from the shark.

* You will run __1 km__ in __1 hour__
* The skark need to swim __&pi; km__ halfway around the island
  * that will take him &pi;/4 = .79 hours = __47 minutes__
  * so the shark will be waiting for 13 minutes for his __tasty meal__.
  * ouch

## Try to find the solution

Then look at the multiple ways to solve it.