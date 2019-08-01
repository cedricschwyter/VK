/**
    Definition of the AssetLoader functor class

    @author       D3PSI
    @version      0.0.1 02.12.2019

    @file         AssetLoader.hpp
    @brief        Definition of the AssetLoader functor class
*/
#pragma once

#include "Model.hpp"
#include "ModelInfo.cpp"

class AssetLoader
{
public:

    AssetLoader(void);

    /**
        Overload instantiation operator to create a functor
    */
    void operator()(void);

    /**
        Returns an std::vector of Model-pointers

        @return     Returns an std::vector< Model* >
    */
    std::vector< Model* > get(void);

private:

    std::vector< Model* >           models;

};

