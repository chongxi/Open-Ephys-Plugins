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

#ifndef LLTHREAD_H
#define LLTHREAD_H
#include <LLProcessorHeaders.h>

namespace PCIeRhythm
{
	class RHD2000Thread;
	class LLOutputThread;

	class LLThread : public GenericLLProcessor
	{
	public:
		LLThread(RHD2000Thread* t);
		~LLThread();
		void prepareToAcquire() override;
		void prepareToStop() override;

	protected:
		void process(float* buffer, uint16* event);
	private:
		RHD2000Thread* dataThread;
		ScopedPointer<LLOutputThread> outputThread;
	};

	class LLOutputThread : public Thread
	{
	public:
		LLOutputThread(RHD2000Thread* t);
		~LLOutputThread();

		void run();
		void send(int word);
		
	private:
		RHD2000Thread* dataThread;
		int output_word {0};
		static const int TIMEOUT_MSEC {10};
	};

}
#endif
