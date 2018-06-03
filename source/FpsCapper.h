#ifndef HAPPYFPSCAP
	#define HAPPYFPSCAP
	
	#define MILISECONDSPERFRAME 16.667
	
	// The milisecodnds at the start of the frame.
	u64 frameStartMiliseconds;
	u64 numberOfFrames;
	u64 tempHold;
	
	#define CAPISENABLED 0
	#if CAPISENABLED
	
		void FpsCapStart(){
			frameStartMiliseconds = getTicks();
		}
		
		void FpsCapWait(){
			// I just hope I only use this at the end of a frame....
			numberOfFrames=numberOfFrames+1;
			
			tempHold = getTicks();
			//printf("%llu;%llu\n",frameStartMiliseconds,tempHold);
			// LIMIT FPS
			if (tempHold-frameStartMiliseconds<MILISECONDSPERFRAME){
				wait( MILISECONDSPERFRAME - (tempHold-frameStartMiliseconds));
			}else{
				//printf("Slowdown %llu\n", tempHold-frameStartMiliseconds);
				//printf("Slowdown %d\n",MILISECONDSPERFRAME - (tempHold-frameStartMiliseconds));
				//printf("Slowdown: %d\n",tempHold-frameStartMiliseconds-MILISECONDSPERFRAME);
			}
			
		}
	#else
		void FpsCapStart(){
		}
		void FpsCapWait(){
		}
	#endif
#endif