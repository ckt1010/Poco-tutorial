#include "Poco/Exception.h"

#include <iostream>

POCO_DECLARE_EXCEPTION( , MyException, Poco::Exception )
POCO_DECLARE_EXCEPTION( , MyFatalException, Poco::Exception )


POCO_IMPLEMENT_EXCEPTION( MyException, Poco::Exception, 
    "Something really bad happened..." )
POCO_IMPLEMENT_EXCEPTION( MyFatalException, Poco::Exception, 
    "Something really really bad happened..." )

void reallyBad () 
{
    throw MyException();
}

void reallyReallyBad ()
{
    throw MyFatalException();
}

int main( void )
{
    try {
        reallyBad();
    } catch ( MyException& ex ) {
        std::cout << ex.displayText() << std::endl;
    } catch ( MyFatalException& ex ) {
        std::cout << ex.displayText() << std::endl;
    }

    try {
        reallyReallyBad();
    } catch ( MyException& ex ) {
        std::cout << ex.displayText() << std::endl;
    } catch ( MyFatalException& ex ) {
        std::cout << ex.displayText() << std::endl;
    }
    // system( "pause" );
    return 0;
}