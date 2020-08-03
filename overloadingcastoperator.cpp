class string;
class sqlite3;
class sqlite3_stmt;

/*example why operator overloading casting was useful*/
class Ptr
{
public:
    // Prepare the statement and initialize its reference counter
    Ptr(sqlite3* apSQLite, string& aQuery);
    // Copy constructor increments the ref counter
    Ptr(const Ptr& aPtr);

    // Move constructor
    Ptr(Ptr&& aPtr);

    // Decrement the ref counter and finalize the sqlite3_stmt when it reaches 0
    ~Ptr();

    /// Inline cast operator returning the pointer to SQLite Database Connection Handle
    operator sqlite3* () const
    {
        return mpSQLite;
    }

    /// Inline cast operator returning the pointer to SQLite Statement Object
    operator sqlite3_stmt* () const
    {
        return mpStmt;
    }

private:
    /// @{ Unused/forbidden copy/assignment operator
    Ptr& operator=(const Ptr& aPtr);
    /// @}

private:
    sqlite3* mpSQLite;    //!< Pointer to SQLite Database Connection Handle
    sqlite3_stmt* mpStmt;      //!< Pointer to SQLite Statement Object
    unsigned int* mpRefCount;  //!< Pointer to the heap allocated reference counter of the sqlite3_stmt
                                 //!< (to share it with Column objects)
};

/* how to do raii in c 


}
*/
#define RAII_VARIABLE(vartype,varname,initval,dtor) \
void _dtor_ ## varname (vartype * v) { dtor(*v); } \
vartype varname __attribute__((cleanup(_dtor_ ## varname))) = (initval)
void raiiExample() {
    RAII_VARIABLE(char*, name, (char*)malloc(32), free);
    strcpy(name, "RAII Example");
    printf("%s\n", name);
}