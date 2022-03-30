# Computer-Cluster-System
Continuous advancements in the scientific and technological fields have created the need for increasingly
capable computational workstations. A computer cluster is a set of computers that work together as one
system to achieve some goal(s). The cluster contains several computers (referred to as workers/machines),
which can vary in capabilities and peripherals. There are different processes that need to be executed, which
are assigned to suitable machines. In this project, we will assume our cluster follows a master/slave
architecture, in which there is a master node that divides the workload among the different nodes (workers).
The master node acts as the central point which assigns processes to machines and receives notifications
from machines after process completion. If we suppose that new processes arrive regularly, we then need
to assign the new processes to the different machines that we have available.
  
  This program simulates process assignment and calculates some related statistics in order to help improve the overall
process.
