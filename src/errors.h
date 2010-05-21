#if !defined(_errors_h_)

#define _errors_h_
#if defined(VMS)
void            err(va_dcl va_alist);
void            fatal_err(va_dcl va_alist);
#else
void            err(const char *emsg, ...);
void            fatal_err(const char *string, ...);
#endif

void            gr_error(const char *lab); // as in gr.hh

void fatal_error(const char *file);
void fatal_error(const char *file, unsigned int line, const char *msg);
void fatal_error(const char *msg);
void fatal_error(const char *msg, const char *msg2);
void fatal_error(const char *msg, const char *msg2, const char *msg3);
void fatal_error(const char *msg, const char *msg2, const char *msg3, const char *msg4);



// Error with filename and line number
#if !defined(gr_Error)
#define gr_Error(err)             \
{                                 \
    string msg;                   \
    msg.append((char*)__FILE__);  \
    msg.append(":");              \
    char num[20];                 \
    sprintf(num, "%d", __LINE__); \
    msg.append(num);              \
    msg.append(": ");             \
    msg.append((err));            \
    gr_error(msg.c_str());        \
}
#endif

#define READ_WORD_ERROR(word) {err("Can't read `\\", (word), "'", "\\");}

#define NUMBER_WORDS_ERROR {err("Wrong number of words in command");}

#define MISSING_WORD_ERROR(word) {err("Missing keyword `\\", (word), "'", "\\");}

#define NO_NEGATIVE_ERROR(word) {err("Cannot have negative value of `\\", (word), "'", "\\");}

#define NO_COLUMN_ERROR(word) {err("Column `\\", (word), "' has no data", "\\");}

#endif				// _errors_h_
