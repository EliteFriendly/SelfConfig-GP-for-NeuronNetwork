#include <iostream>

using namespace std;

#pragma once
class ComputingLimitation
{
  private:
    int initialComputingLimitation = 0;

  public:
    void setComputingLimitation(int initialComputingLimitation)
    {
        this->initialComputingLimitation = initialComputingLimitation;
    }
    int getComputingLimitation()
    {
        return initialComputingLimitation;
    }
    bool useComputing()
    {
        if (initialComputingLimitation > 0)
        {
            initialComputingLimitation--;
            return true;
        }
        else
        {
            return false;
        }
    }
};