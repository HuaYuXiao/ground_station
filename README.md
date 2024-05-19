# prometheus_station

The prometheus_station package, modified from [prometheus_station](https://github.com/amov-lab/Prometheus/tree/v1.1/Modules/ground_station)


## Release Note

- v1.2.0: 
  - support `POS_VEL_ACC` move mode
  - catch invalid input with `UNDEFINED`
- v1.0.1: support `launch`


## Compilation

```bash
catkin_make install --source Modules/ground_station --build build/ground_station
```


## Launch

```bash
roslaunch ground_station simulation.launch
```
