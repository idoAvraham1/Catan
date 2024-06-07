cmake_minimum_required(VERSION 3.27)
project(Catan)

set(CMAKE_CXX_STANDARD 17)

add_executable(Catan main.cpp
        Catan.hpp
        Catan.cpp
        Board.cpp
        Board.hpp
        Player.cpp
        Player.hpp
        Catan_API.hpp
        doctest.h
        TestCounter.cpp
        Test.cpp
        DevelopmentCard.cpp
        DevelopmentCard.hpp
        TestPlayer.hpp
        CardDeck.cpp
        CardDeck.hpp
        DevelopmentCardManagement.cpp
        DevelopmentCardManagement.hpp
        Edge.cpp
        Edge.hpp
        Vertex.cpp
        Vertex.hpp
        Tile.cpp
        Tile.hpp
        ResourceManagement.cpp
        ResourceManagement.hpp
)
