# CANOpenNode_OD
Testing environment for developing different implementations of CANOpen Node Object dictionary.
The main idea is to define an API to access the Object Dictionary. The API will hide the implementation details of the Object Dictionary, so in case of specific needs, the user can reimplement the Object Dictionary and still use the CANOpen Node stack.

## OD implementations
Currently there are following Object Dictionary implementations

### Original OD impelementation
In od_interface_original the original OD implementation is located. It is there so it can be compared to new implementations. It is not fully operationa atm.

### Linked list OD implementations
There are two linked list implementations. The od_interface_linked_list is a prof of concept for the linked list implementatio. It is superseeded by the od_interface_linked_list_v2 impelementation.

#### od_interface_linked_list
It is fully functional implementation with unit tests implemented in test/canopen/od_linked_list_v2/ (running make clean; make all in this folder will run the unit tests) and usage example in od_interface_linked_list_v2/usage_example (running make clean; make all in this folder will run the example which will use the od_description_generator to print the OD to the stdout

## OD description generator
The idea of the description generator is to have a tool, which will read the actual OD and make an OD description (EDS,...) 
TODO: Write the description and main idea behind the description generator
