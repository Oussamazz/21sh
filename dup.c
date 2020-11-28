

#include<stdio.h> 
#include <unistd.h> 
#include <fcntl.h> 
  
int main() 
{ 
    int file_desc = open("dup.txt", O_WRONLY | O_APPEND | ); 
      
    if(file_desc < 0) 
        printf("Error opening the file\n"); 
    int copy_desc = dup(file_desc); 
	
	ft_putendl_fd("this is using fd == copy_desc", copy_desc);

	ft_putendl_fd("this is using fd == file_descp ", file_desc);	
	return 0; 
}
