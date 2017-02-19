//独自allocatorを使いたい場合は、以下のマクロを有効にすると、
//local_allocator.hpp内部のxc::local_allocatorを利用してくれるようになる
//#define XC_LOCAL_ALLOCATOR
#if defined(_MSC_VER) || defined(__clang__ )
#	pragma message("###############################")
#	pragma message("##                           ##")
#	pragma message("##        DEBUG MODE         ##")
#	pragma message("##                           ##")
#	pragma message("###############################")
#else
#	pragma message("###############################")
#	pragma message("##                           ##")
#	pragma message("##       RELEASE MODE        ##")
#	pragma message("##                           ##")
#	pragma message("###############################")
#endif
