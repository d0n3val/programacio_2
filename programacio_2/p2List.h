#ifndef __P2LIST_H__
  #define __P2LIST_H__

#include "p2Defs.h"

/**
* Contains items from double linked list
*/
template<class tdata>
struct p2list_item
{
  tdata                 data;
  p2list_item<tdata>*   next;
  p2list_item<tdata>*   prev;

  inline p2list_item( const tdata& _data )
  {
    data = _data;
    next = prev = NULL;
  }

  ~p2list_item()
  {
  }
};

/**
* Manages a double linked list
*/
template<class tdata>
class p2list
{

public:
	
	p2list_item<tdata>*   start;
	p2list_item<tdata>*   end;

protected:

    long  size;

public:

    /**
    * Constructor
    */
    inline p2list()
    {
      start = end = NULL;
      size = 0;
    }

    /**
    * Destructor
    */
    ~p2list()
    {
		clear();
    }

    /**
    * Get Size
    */
	long count()
	{
		return size;
	}

    /**
    * Add new item
    */
    void add( const tdata& item )
    {
      p2list_item<tdata>*   p_data_item;

      p_data_item = new p2list_item < tdata > ( item );

      if( start == NULL )
      {
        start = end = p_data_item;
      }
      else
      {
        p_data_item->prev = end;
        end->next = p_data_item;
        end = p_data_item;
      }

      ++size;
    }

    /**
    * Deletes an item from the list
    */
    bool del( p2list_item<tdata>* item = NULL )
    {
      assert( item != NULL );

      // Now reconstruct the list
      if( item->prev != NULL )
      {
        item->prev->next = item->next;
        if( item->next != NULL )
        {
          item->next->prev = item->prev;
        }
        else
        {
          end = item->prev;
        }
      }
      else
      {
        if( item->next )
        {
          item->next->prev = NULL;
          start = item->next;
        }
        else
        {
          start = end = NULL;
        }
      }

      RELEASE( item );
      --size;

      return( true );
    }

    /**
    * Returns the first position of the list
    */
    bool first( tdata* item ) const
    {
      if( size <= 0 )
      {
        return( false );
      }

      item = &start->data;
      return( true );
    }

    /**
    * Returns the last position of the list
    */
    bool last( tdata* item ) const
    {
      if( size <= 0 )
      {
        return( false );
      }

      item = &end->data;
      return( true );
    }

    /**
    * Destroy and free all mem
    */
    void clear()
    {
      p2list_item<tdata>*   p_data;
      p2list_item<tdata>*   p_next;

      p_data = start;
      while( p_data != NULL )
      {
        p_next = p_data->next;
        RELEASE( p_data );
        p_data = p_next;
      }
      start = end = NULL;
      size = 0;
    }

	/**
    * read / write operator access directly to a position in the list
    */
    tdata & operator  [] (const long index)
    {
      long                  pos;
      p2list_item<tdata>*   p_item;

      pos = 0;
      p_item = start;

      while( p_item != NULL )
      {
        if( pos == index )
        {
          break;
        }

        ++pos;
        p_item = p_item->next;
      }

      return( p_item->data );
    }
};
#endif /*__p2list_H__*/