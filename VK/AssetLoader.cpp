/**
    Implementation of the AssetLoader functor class

    @author       D3PSI
    @version      0.0.1 02.12.2019

    @file         AssetLoader.cpp
    @brief        Implementation of the AssetLoader functor class
*/
#include "AssetLoader.hpp"
#include "VK.hpp"


AssetLoader::AssetLoader() {



}

void AssetLoader::operator()() {

    std::unique_lock< std::mutex> lock(vk::engine->modelLoadingQueueMutex);

    while (!vk::engine->done) {

        while (!vk::engine->notified) {  // loop to avoid spurious wakeups

            vk::engine->modelLoadingQueueCondVar.wait(lock);

        }

    }

    while (!vk::engine->modelLoadingQueue.empty()) {
    
        Model* model = new Model(vk::engine->modelLoadingQueue.front().path, vk::engine->modelLoadingQueue.front().pipeline, vk::engine->modelLoadingQueue.front().lib);
        vk::engine->modelLoadingQueue.pop();
        models.push_back(model);

    }
    
    vk::engine->notified = false;

}

std::vector< Model* > AssetLoader::get() {

    return models;

}