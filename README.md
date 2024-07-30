Project where we had to design a Student TA system.

This program simulates a scenario where a teaching assistant helps students using multithreading in C. I modeled it after a general scenario where a TA helps students working on assignments. The simulation involves:

A limited number of chairs within the hallway for students who may have to wait in case the TA is busy.
Students come randomly to get help from the TA.
The TA can only help one student at a time. Other students have to wait for their turn.
The pthread library is used to manage threads, and synchronization is achieved by using semaphores.

# TA and Students Multithreading Simulation

## Description

This program simulates a scenario where a teaching assistant helps students using multithreading in C. It is a use case where a TA helps students working on assignments. The simulation involves:

- A limited number of chairs within the hallway for students who may have to wait in case the TA is busy.
- Students come randomly to get help from the TA.
- The TA can only help one student at a time. Other students have to wait for their turn.

The pthread library is used to manage threads, and synchronization is achieved by using semaphores.

## How It Works

### Key Components

1. **Libraries and Definitions**:
    - `pthread.h`: This contains the thread functions.
    - `semaphore.h`: This contains the semaphore functions.
    - `stdio.h`: This contains the input and output functions.
    - `stdlib.h`: This contains standard library functions such as `rand()`.
    - `unistd.h`: Used for the `sleep()` function.
    - `NUM_CHAIRS`: The number of chairs in the waiting room.

2. **Global Variables**:
    - `mutex`: This is a mutex lock used to protect all the critical sections.
    - `studentsSem`: Semaphores are used to check the number of waiting students.
    - `taSem`: Semaphores are used for the TA availability.
    - `numStudents`: A count of the total number of students.
    - `servedStudents`: This maintains a count of the students served.
    - `waitingStudents`: This maintains a count of the students currently waiting.

### Thread Functions

1. **TA Thread**:
    - The TA will wait until any student signals that they want to be helped.
    - It enters a critical section and decrements `waitingStudents`.
    - Signals the student that the TA is ready.
    - Simulates helping the student by sleeping for a random time.
    - Increment `servedStudents` counter.
    - Repeat the above step until all students are served.

2. **Student Thread**:
    - Acts as a student who is programming.
    - Control structure to check if it requires help and the number of chairs are available in the hallways.
    - If any chairs are vacant, then waits in the hallway and signal TA.
    - Waits for the TA to be free and then gets help which is simulated.
    - If all the chairs are filled, he waits for a random amount of time and checks again.

### Main Function

- It accepts number of students as an argument from the command line (for the user)
- It initializes the mutex and semaphores
- It creates the TA thread
- It creates threads for all the students
- It waits for all student threads to finish (joins them)
- It cancels and joins TA thread
- It destroys the mutex and semaphores.


## Note

- Number of chairs in the hallway is defined by `NUM_CHAIRS`.
- The program employs random sleep times to display a variability between student arrival and the time taken to help each student. It's a pretty basic and straightforward example of how to use threads and semaphores to control concurrency in C.


