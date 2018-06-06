




/*

    st_Message Mailbox::getMessage(uint8_t i_u8TaskID, uint16_t i_u16MessageCode)
    {
        //Temporal used variables

        st_Message l_stControlMessage2Read; //To check number of valid messages
        st_Message l_stOverwriteMessage;    //Structure needed to mark the read message as invalid
        st_Message l_stMessage2Return;      //Message to return

        //Load Control Message

        l_stControlMessage2Read = *( MessageAccessRead(i_u8TaskID, MAILBOX_INFO_SLOT) );


        if( l_stMessage2Read.u8NumberOfMessages > 0)
        {
            //Find Valid Message to read
            uint8_t l_u8Slot2Read = FindMessageSlot(i_u8TaskID, VALID_MESSAGE, i_u16MessageCode);

            l_stMessage2Return = *( MessageAccessRead(i_u8TaskID, l_u8Slot2Read) );

            l_stOverwriteMessage = l_stMessage2Return;

            //Mark message as invalid so it can be taken out of queue

            l_stOverwriteMessage.bMessageValid = INVALID_MESSAGE;

            //Write the changes on queued message
            *(*(l_pListOfTasksMessages + i_u8TaskID) + l_u8Slot2Read) = l_stOverwriteMessage;

            //Reduce number of valid messages since since the message was read
            l_stControlMessage2Read.u8NumberOfMessages--;

            //Write the changes on the control message
            *(*(l_pListOfTasksMessages + i_u8TaskID) + l_u8Slot2Read) = l_stOverwriteMessage;

            //Free used memory
            free(&l_stControlMessage2Read);
            free(&l_stOverwriteMessage);

            return(l_stMessage2Return);
        }

        return(m_stDefaultMessage);
    }
*/
