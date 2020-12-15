## Chat between 5 processes  semaphores_and_shared-memory
This is chat simulation with semaphores and shared memory between 5 processes

p1 is the first person in chat 
enc is a check sum process to check if message send and resend for p2
chan is a process with rand that can change the message (noise)
enc2 is a check sum process to check if message send and resend for p1
p2 is the second person in chat
Messages past through arguments at p1-p2.
 
Compile: make && mkdir key
Delete: make clean
Execution: Open five terminals. ./p1 This is a message from p1 TERM , ./enc , ./chan , ./enc2 , p2 THIS IS A MESSAGE FROM p2 TERM

When you TERM  the execution end. try too exec ./p1 This is a TERM message from p1 , ./enc , ./chan , ./enc2 , p2 THIS IS A MESSAGE FROM p2 TERM
