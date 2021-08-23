R"(

void
kernel
Clear( global uint8* iDst )
{
    iDst[ get_global_id( 0 ) ] = 0;
}

)"

