
#define SAFE_DELETE(a) if( (a) != nullptr ) delete (a); (a) = nullptr;