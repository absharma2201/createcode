/* file directory traversal c++ coding
 */
#include <iostream>
using namespace std;

struct FileNode
{
    std::string value;
    FileNode* parent;
    FileNode* childListHead;
    FileNode* nextChild;

    FileNode( ) :
        childListHead( nullptr ),
        nextChild( nullptr ),
        parent( nullptr )
    {}

    FileNode( const std::string &value, FileNode *parent ) :
        childListHead( nullptr ),
        nextChild( nullptr ),
        parent( parent ),
        value( value )
    {}

    FileNode* lastChild()
    {
        FileNode *nextChild = childListHead, *child = nullptr;

        while( nextChild != nullptr )
        {
            child = nextChild;
            nextChild = nextChild->nextChild;
        }

        return child;
    }

    FileNode* find( const std::string &entry )
    {
        FileNode *tmp = childListHead;

        while( tmp != nullptr && tmp->value != entry ) {
            tmp = tmp->nextChild;
        }
        
        return tmp;
    }

    void add( const std::string &value )
    {
        
        if( childListHead == nullptr )
            childListHead = new FileNode( value, this );
        else
            lastChild( )->nextChild = new FileNode( value, this );
    }
};

bool addPath( std::string rootPath, const std::string &path, FileNode *currentFolder )
{
    std::string nextFolder;
    size_t pos = path.find( '/' );
    nextFolder = ( pos != std::string::npos ) ? path.substr( 0, pos ) : "";

    if( nextFolder.length( ) > 0 )
    {
        FileNode *findedEntry;
        findedEntry = currentFolder->find( nextFolder );

        if( findedEntry == nullptr ) {
            findedEntry = new FileNode( nextFolder, nullptr );
            findedEntry->nextChild = currentFolder->childListHead;
            currentFolder->childListHead = findedEntry;
            cout<<rootPath+nextFolder<<endl;
        } 
        addPath( rootPath+nextFolder+"/", path.substr( pos + 1 ), findedEntry );
    }
    else
    {
        if( currentFolder->find( path ) != nullptr )
            return false;

        cout<<rootPath+path<<endl;
        currentFolder->add( path );

    }

    return true;
}

void addFile(const std::string &path, FileNode **currentFolder) {
    if((*currentFolder) != nullptr) {
        addPath("/", path.substr( path.find( '/' ) + 1 ), *currentFolder);
    } else {
        *currentFolder = new FileNode( "/", nullptr );
        cout<<"/"<<endl;
        addPath("/", path.substr( path.find( '/' ) + 1 ), *currentFolder);
    }
}

int main( void )
{
    FileNode* root = nullptr;
    cout<<"___________________________________________________"<<endl;
    addFile( "/folder1/folder2_1/file.txt", &root );
    cout<<"___________________________________________________"<<endl;
    addFile( "/folder1/folder2_1/file2.txt", &root );
    cout<<"___________________________________________________"<<endl;
    addFile( "/folder1/folder2_2/file2.txt", &root );
    cout<<"___________________________________________________"<<endl;
    addFile( "/folder2/folder3/file3.txt", &root );
    cout<<"___________________________________________________"<<endl;
    return 0;
}
