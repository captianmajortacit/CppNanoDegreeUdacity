#include <iostream>
#include <random>
#ifndef TRAFFICLIGHT_H
 #include "TrafficLight.h"
#endif

/* Implementation of class "cMessageQueue" */

 
template <typename T>
T cMessageQueue<T>::receive()
{
    // FP.5a : The method receive should use std::unique_lock<std::mutex> and _condition.wait() 
    // to wait for and receive new messages and pull them from the queue using move semantics. 
    // The received object should then be returned by the receive function.
    
    //perform the vector modificaiton under the lock
    std::unique_lock<std::mutex> uLock(_mutex);
    _cond.wait(uLock,[this](){return !_queue.empty(); }); //pass unique_lock to condition variable

    //remove last vector element from the queue
    T msg = std::move(_queue.back());
    //_queue.pop_back();
    _queue.clear();
    //std::cout<<"Message #"<<msg<<" recieved"<<std::endl; //debugging
    return msg;
}

template <typename T>
void cMessageQueue<T>::send(T &&msg)
{
    // FP.4a : The method send should use the mechanisms std::lock_guard<std::mutex> 
    // as well as _condition.notify_one() to add a new message to the queue and afterwards send a notification.
    
    //perform vector modification under the lock
    std::lock_guard<std::mutex> uLock(_mutex);
    //add vector to the queue
    //std::cout<<"Message #"<<msg<<" will be added to the queue"<<std::endl; //debugging
    _queue.push_back(std::move(msg));
    _cond.notify_one(); //notify client after pushing new Traffic Light into the vector

}



/* Implementation of class "TrafficLight" */ 
TrafficLight::TrafficLight()
{
    _currentPhase = TrafficLightPhase::red;
}

void TrafficLight::waitForGreen()
{
    // FP.5b : add the implementation of the method waitForGreen, in which an infinite while-loop 
    // runs and repeatedly calls the receive function on the message queue. 
    // Once it receives TrafficLightPhase::green, the method returns.
    while(true)
    {
        if(_messageQueue->receive()== TrafficLightPhase::green)
        {
            return;
        }
    }
}

TrafficLightPhase TrafficLight::getCurrentPhase()
{
    return _currentPhase;
}

void TrafficLight::simulate()
{
    // FP.2b : Finally, the private method „cycleThroughPhases“ should be started in a thread when the public method „simulate“ is called. To do this, use the thread queue in the base class. 
    threads.emplace_back(std::thread(&TrafficLight::cycleThroughPhases, this));
}

// virtual function which is executed in a thread
void TrafficLight::cycleThroughPhases()
{
    // FP.2a : Implement the function with an infinite loop that measures the time between two loop cycles 
    // and toggles the current phase of the traffic light between red and green and sends an update method 
    // to the message queue using move semantics. The cycle duration should be a random value between 4 and 6 seconds. 
    // Also, the while-loop should use std::this_thread::sleep_for to wait 1ms between two cycles. 
    std::chrono::time_point<std::chrono::system_clock> loc_previousTime = std::chrono::system_clock::now();
    std::random_device loc_Random;
    std::mt19937 gen(loc_Random());
    std::uniform_real_distribution<> loc_distribution(4000,6000);//seconds to milliseconds conversion
    while(true)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        long loc_timeSinceLastChange = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - loc_previousTime).count();
        //When _currentPhase is yellow, wait for 1 second else wait between 4 and 6 seconds
        int loc_randomDuration = (_currentPhase == TrafficLightPhase::yellow) ? 1000:loc_distribution(gen);
        //debugging
        //std::cout<<"RandomDuration: "<<loc_randomDuration<<" Time Since Last Change: "<<loc_timeSinceLastChange<<" currentPhase:"<<_currentPhase<<std::endl;
        if(loc_timeSinceLastChange >= loc_randomDuration)
        {
            loc_previousTime = std::chrono::system_clock::now();
            if(_currentPhase == TrafficLightPhase::red)
            {
                _currentPhase = TrafficLightPhase::green;
            }
            else if (_currentPhase == TrafficLightPhase::green)
            {
                _currentPhase = TrafficLightPhase::yellow;//Switch to yellow after green is active for a random duration between 4 and 6 seconds 
            }
            else
            {
                _currentPhase = TrafficLightPhase::red;
            }
            _messageQueue->send(std::move(_currentPhase));
        }            
    }
}
