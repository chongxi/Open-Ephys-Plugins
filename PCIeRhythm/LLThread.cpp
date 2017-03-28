/*
------------------------------------------------------------------

This file is part of the Open Ephys GUI
Copyright (C) 2013 Open Ephys

------------------------------------------------------------------

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/


#include "LLThread.h"
#include "RHD2000Thread.h"

#define THRESHOLD_CHECK 1000.0f

using namespace PCIeRhythm;

LLThread::LLThread(RHD2000Thread* t) : dataThread(t)
{
	outputThread = new LLOutputThread(t);
}

LLThread::~LLThread()
{}

void LLThread::prepareToAcquire()
{
	//std::cout << "PRIO: " << setPriority(7) << std::endl;
	outputThread->startThread();
}

void LLThread::prepareToStop()
{
	if (!outputThread->stopThread(100))
		std::cerr << "Output thread forcefully closed" << std::endl;
}

void LLThread::process(float* buffer, uint16* event) //Called once per sample
{
	/*
	for (int i=0; i < nChannels; i++) //nChannels is a protected member with the number of channels
	{
		whatever(buffer[i]) //channels in the buffer can be accessed by simple indexing
	}
	*/
	double temp =0;
	for (int i=0; i < 16; i++)
	{
		for (int j=0; j < nChannels; j++)
			temp += buffer[j]/(i+1);
	}
// std::cout << "Aux input: " << *event << std::endl;
	//std::cout << temp << std::endl;
	//std::cout << buffer[0] << std::endl;
	if (buffer[0] > THRESHOLD_CHECK)
		//dataThread->setOutputSigs(0xFFFFFFFF);
		outputThread->send(0xFFFFFFFF);
	else
		//dataThread->setOutputSigs(0x0);
		outputThread->send(0x00000000);
	buffer[1] = temp;

}


LLOutputThread::LLOutputThread(RHD2000Thread* t) : 
	Thread("OutputThread"), 
	dataThread(t) 
{ }

LLOutputThread::~LLOutputThread()
{ }

void LLOutputThread::run() {
	
	while (!threadShouldExit()) {

		if (!wait(TIMEOUT_MSEC))
			continue;

		dataThread->setOutputSigs(output_word);
	}
}

void LLOutputThread::send(int word) 
{
	output_word = word;
	notify();
}






