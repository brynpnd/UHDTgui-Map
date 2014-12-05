/*  File: container.c
 *  Date: 12 Sep 06
 *  Rev:  25 Sep 07
 *
 *  Container class definitions - version 2
 */

#include <stdlib.h>
#include <stdio.h>
#include "container.h"

Container::Container()
/*  Given: None
    Returns: Container that has been created a an empty container
    Purpose: Initializes a Container object
 */
{	clear();

}

void Container::clear()
/*  Given: None
 *  Returns: None
 *  Purpose: Removes all entries from the container, and resets count to 0
 */
{	count = 0;

}

bool Container::isempty() const
/*  Given: None
    Returns: True or False
    Purpose: Returns true if the container is empty, and false otherwise
 */
{
    // if the count is equal to zero, it will return true
    return(count == 0);

}

bool Container::isfull() const
/*  Given: None
 *  Returns: True or False
    Purpose: Returns true if the container is full, and false otherwise
 */
{
    // if the count is greater than the maximum size allowed, it will return true
    return(count + 1 == MAXSIZE);
}

int Container::size() const
/*  Given: None
    Returns: Size of the container
    Purpose: Returns the size of the container
 */
{
    return count;
}

void Container::traverse(void(*Visit)(Entry))
/*  Given: Function pointer to each element of the Container
    Returns: None
    Purpose: Apply a function to each element of the Container without modifying them
 */
{
    // apply the visit function pointer to each element of the container
    for(int n = 0; n < size(); n++)
         Visit(elements[n]);

}

void Container::insert(Entry x, Position p)
/*  Given: Entry to be inserted into container, and the position to insert it to
    Returns: None
    Purpose: Insert the entry x into the container at position p
*/
{
    if(p == -1)
    {	elements[size()] = x;
        count++;
    }

    else if(p < 0 || p > size());

    else
    {	int temp_size = size();

        while(temp_size > p)
        {
            elements[temp_size] = elements[temp_size-1];
            temp_size--;
        }

        if(count < MAXSIZE)
        {	elements[p] = x;
            count++;
        }

        else
            printf("Error! Too many entries.");
    }

}

Entry Container::remove(Position p)
/*  Given: Position of an entry
    Returns: Entry that was removed
    Purpose: Remove the entry at position p of the container
*/
{
    if(p < 0 || p >= size())
    {	printf("Error! Illegal position.\n");
        return 0;
    }

    for(int temp_p = p; temp_p + 1 < size(); temp_p++)
        elements[temp_p] = elements[temp_p+1];


    count--;
    return p;
}

Entry Container::get(Position P) const
/*
    Given: Position of an entry
    Returns: Entry at position p
    Purpose: Retrieve the entry at position p
*/

{	if(P < 0 || P >= size())
    {	printf("Error! Illegal position.\n");
        return 0;
    }

    return elements[P];

}

void Container::replace(Entry x, Position p)
/*
    Given: Entry to replace what's currently in position p
    Returns: None
    Purpose: Replace the entry at position p with entry x
*/
{
    if(p < 0 || p >= size())
        printf("Error! Illegal position.\n");
    else
        elements[p] = x;

}

