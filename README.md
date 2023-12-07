# MPI_mini_programs_examples
MPI program examples

## Requirements
* Linux
* MPI

### Installation process(ONLY FOR LINUX)
1. Add this line into etc/apt/sources.list deb http://gr.archive.ubuntu.com/ubuntu xenial main universe 
2. Write this command sudo apt-get install libcr-dev mpich mpich-doc
3. Ready

### Run
* mpicc name.c -o name
* mpirun np -(process number ex. 4) ./name 
