# Vary Resolution

## Number of segments

![binary_points](binary_points.png)

Unlike the [simulated annealing][./aresolution.md) algorithm, the binry algorithm is sensitive to initial many factors.

## Rate of descent

Technically a __binary__ search will decrease the purtubation in half each step. That .5 factor can be varied:

![binary_descent](binary_descent.png)

But the result is no smoother

## Smoothing

In theory, smoothing should give a less jagged contour.

![binary_smooth](binary_smooth.png)

In these cases, it seems that the abrupt changes are seen at start and end, not just start.

## Penalty Weighting

Should have no bearing if a solution is findable, but test:

![binary_penalty](binary_penalty.png)

In contrast, the Simulated Annealling is insensitive to the penalty multiple:

![anneal_penalty](anneal_penalty.png)