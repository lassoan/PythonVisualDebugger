
#include "priv_coreprecompiled.h"

#ifdef	WIN32

#include "errorhandler.h"

#include <vector>
#include <string>

namespace he = HumongousEntertainment;

namespace HumongousEntertainment
{
	typedef std::vector<std::string> ErrorList;

	// ErrorHandler object.  Intended to print and log errors and warnings
	// in command-line based tools in a standard manner.

	class ErrorHandler
	{
	public:
		ErrorHandler();
		void FatalError(const std::string& msg);
		void Error(const std::string& msg);
		void Warning(const std::string& msg);
		void DisableErrorSummary();
		~ErrorHandler();
				
	private:
		ErrorList m_EList;
		bool m_bPrintErrorSummary;
		bool m_bEndedOnError;
		int m_nErrors;
		int m_nWarnings;

	};	
	
	// Class member definitions
	// =============================================================================

	ErrorHandler::ErrorHandler()
	{
		m_bEndedOnError = false;
		m_bPrintErrorSummary = true;
		m_nErrors = 0;
		m_nWarnings = 0;

	}

	ErrorHandler::~ErrorHandler()
	{	
		// Don't print the list if we just ended on an error, it will just print
		// the same thing over again

		if ((!m_bEndedOnError) && (m_EList.size() > 0) && (m_bPrintErrorSummary))
		{
			printf("\n\nError Summary:  %d error(s) %d warning(s)\n\n", m_nErrors, m_nWarnings);
			
			for (ErrorList::iterator i = m_EList.begin(); i != m_EList.end(); ++i)
			{
				printf("%s\n", i->c_str());
			}
		}		
	}

	
	void ErrorHandler::FatalError(const std::string& msg)
	{		
		printf("\n%s\n", msg.c_str());	
		m_EList.push_back(msg);

		m_bEndedOnError = true;
		++m_nErrors;
		exit(EXIT_FAILURE);		
	}
	

	void ErrorHandler::Error(const std::string& msg)
	{
		// This is the same as a fatal error, but does not exit the program.
				
		printf("\n%s\n", msg.c_str());	
		m_EList.push_back(msg);

		++m_nErrors;
	}


	void ErrorHandler::Warning(const std::string& msg)
	{		
		printf("\n%s\n", msg.c_str());	
		m_EList.push_back(msg);
		
		++m_nWarnings;
	}


	void ErrorHandler::DisableErrorSummary()
	{
		m_bPrintErrorSummary = false;
	}


	// This instance of the class will be destroyed when the program exits
	// this will run the destructor and will print out all of the error messages
	// that have been saved up

	static ErrorHandler& GetErrorHandler()
	{
		static ErrorHandler eh;
		return eh;
	}


	std::string vstringprintf(const char* format, va_list args)
	{
		// Allocate a printf buffer.
		std::vector<char>      buffer(500);
		int result;
		
		do
		{
			result = _vsnprintf(&buffer[0], buffer.size() - 1, format, args);
			// If we didn't have enough room, keep increasing the buffer size until
			// we do have enough room.
			if (result == -1)
				buffer.resize(buffer.size() * 2);
		} while (result == -1);
		
		// Make sure the buffer is null terminated - I don't trust _vsnprintf to
		// do this in the boundary cases.
		buffer[buffer.size() - 1] = 0;

		return &buffer[0];
	}
		



	std::string stringprintf(char const *format, ...)
	{
		va_list args; 
		va_start(args, format);
		
		std::string strReturn = vstringprintf(format, args);		
 
		va_end(args);

		return strReturn;		
	}






} // End of he namespace



// These functions are not in the he namespace so they can be called from .c files
#if defined (__cplusplus)
extern "C" {
#endif

void __cdecl HE_FatalError(const char *msg, ...)
{
	va_list args;
	va_start(args, msg);
	
	he::GetErrorHandler().FatalError("FATAL ERROR: " + he::vstringprintf(msg, args));
	
	va_end(args);	
}


void __cdecl HE_Warning(const char *msg, ...)
{
	va_list args;
	va_start(args, msg);
	
	he::GetErrorHandler().Warning("WARNING: " + he::vstringprintf(msg, args));
	va_end(args);
}


void __cdecl HE_Error(const char *msg, ...)
{
	va_list args;
	va_start(args, msg);
	
	he::GetErrorHandler().Error("ERROR: " + he::vstringprintf(msg, args));
	va_end(args);
}


void __cdecl HE_DisableErrorSummary()
{
	he::GetErrorHandler().DisableErrorSummary();
}

#if defined (__cplusplus)
} // end extern "C"
#endif

#endif	// WIN32
