Here are the information requied to run the programs.


(1) Serial Program
	file name - SerialProgram.c
	Complier command - gcc -g -Wall -o serial_program SerialProgram.c -ls
	execution command - ./serial_program <n> <m> <m_member> <m_insert> <m_delete> <sample size> 

	(Ex:-./serial_program 1000 10000 0.90 0.05 0.05 20 8)

(1) One mutex for entire list
	file name - MutexProgram.c
	Complier command -  gcc -g -Wall -o mutex_program MutexParallelProgram.c -lm -lpthread 
	execution command - ./mutex_program <n> <m> <m_member> <m_insert> <m_delete> <sample size> <thread count>

	(Ex:-./mutex_program 1000 10000 0.90 0.05 0.05 20 8)

(1) Read write lock
	file name - RWLockProgram.c
	Complier command - gcc -g -Wall -o rwlock_program RWLockProgram.c -lm -lpthread 
	execution command - ./rwlock_program <n> <m> <m_member> <m_insert> <m_delete> <sample size> <thread count>
	
	(Ex:-./rwlock_program 1000 10000 0.90 0.05 0.05 20 8)
 


