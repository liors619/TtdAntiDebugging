//BOOL isDebugged = TRUE;
//__try
//{
//    __asm
//    {
//        pushfd
//        or dword ptr[esp], 0x100 // set the Trap Flag 
//        popfd                    // Load the value into EFLAGS register
//        nop
//    }
//}
//__except (EXCEPTION_EXECUTE_HANDLER)
//{
//    // If an exception has been raised – debugger is not present
//    isDebugged = FALSE;
//}
//if (isDebugged)
//{
//    std::cout << "Stop debugging program!" << std::endl;
//    exit(-1);
//} It was originally published on https ://www.apriorit.com/