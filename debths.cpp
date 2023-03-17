#include <iostream>
#include <unistd.h>
#include <sys/ptrace.h>
#include <wait.h>
#include <sys/user.h>

int main (int argc, char** argv) {
    pid_t subp = fork();
    if ( !subp ) {
        char* const args[] = {NULL};
        execve(argv[1], args, NULL);
    }

    int wstatus;
    ptrace(PTRACE_ATTACH, subp, NULL, NULL);
    waitpid(subp, &wstatus, 0);

    struct user_regs_struct sregs;
    long startpoint = 0x0000000000001080;
    long mainf_address = 0x0000000000001169;

    if ( WIFSTOPPED(wstatus) ) {
        puts("Child was stopped...");
        long dk1 = ptrace(PTRACE_PEEKTEXT, subp, mainf_address, NULL);
        long dk2 = (dk1 & 0xFF) | 0xCC;
        ptrace(PTRACE_POKETEXT, subp, dk1, dk2);
        puts("BREAK POINT SET!");

        ptrace(PTRACE_GETREGS, subp, NULL, &sregs);
        std::cout << "rsp: " << sregs.rsp << '\n';
        getchar();
    }
    return 0;
}
