float pos_x=0;
float pos_y=0;
float zangle=0;
float xangle=0;
float yangle=0;
float w_z=0;
void USART3_IRQHandler(void)
{   
    static uint8_tch;
    static union
    {
     uint8_tdata[24];
     floatActVal[6];
    }posture;
    static uint8_tcount=0;
    static uint8_ti=0;
    OS_CPU_SR  cpu_sr;
   OS_ENTER_CRITICAL();/* Tell uC/OS-II that we are starting an ISR*/
    OSIntNesting++;
   OS_EXIT_CRITICAL();

   if(USART_GetITStatus(USART3, USART_IT_RXNE)==SET)   
    {
       USART_ClearITPendingBit( USART3,USART_IT_RXNE);
       ch=USART_ReceiveData(USART3);
        switch(count)
         {
             case0:
                if(ch==0x0d)
                    count++;
                else
                    count=0;
                break;                 
             case1:
                if(ch==0x0a)
                 {
                    i=0;
                    count++;
                 }
                else if(ch==0x0d);
                else
                    count=0;
                break;                 
             case2:
                posture.data=ch;
                 i++;
                if(i>=24)
                 {
                    i=0;
                    count++;
                 }
                break;                 
             case3:
                if(ch==0x0a)
                    count++;
                 else
                    count=0;
                break;                 
             case4:
                if(ch==0x0d)
                 {
                 zangle=posture.ActVal[0];
                 xangle=posture.ActVal[1];
                  yangle=posture.ActVal[2];
                     pos_x =posture.ActVal[3];
                     pos_y =posture.ActVal[4];
                     w_z   =posture.ActVal[5];
                 timede=run;
                 run=0;
                 }
               count=0;
                break;   
            default:
                count=0;
              break;        
         }
     }
    OSIntExit();
}