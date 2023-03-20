#include <catch2/catch_all.hpp>

TEST_CASE("This is a Test","[test]"){
    REQUIRE(true);
}

int main(int argc, char* argv[]) {
    Catch::Session session;
    int returnCode = session.applyCommandLine( argc, argv );
    if( returnCode != 0 )
        return returnCode;
    return session.run();
}