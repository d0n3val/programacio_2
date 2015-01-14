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
    const long&           size;
    p2list_item<tdata>*   start;
    p2list_item<tdata>*   end;
  protected:
    long  _size;
  public:

    /**
    * Constructor
    */
    inline p2list() :
    size( _size )
    {
      start = end = NULL;
      _size = 0;
    }

    /**
    * Destructor
    */
    ~p2list()
    {
      clear();
    }

    /**
    * Add new item
    */
    inline void add( const tdata& item )
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

      ++_size;
    }

    /**
    * Deletes an item from the list
    */
    inline bool del( p2list_item<tdata>* item = NULL )
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

      delete( item );
      --_size;

      return( true );
    }

    /**
    * Returns the first position of the list
    */
    inline bool first( tdata* item ) const
    {
      if( _size <= 0 )
      {
        return( false );
      }

      item = &start->data;
      return( true );
    }

    /**
    * Returns the last position of the list
    */
    inline bool last( tdata* item ) const
    {
      if( _size <= 0 )
      {
        return( false );
      }

      item = &end->data;
      return( true );
    }

    /**
    * Destroy and free all mem
    */
    inline void clear()
    {
      p2list_item<tdata>*   p_data;
      p2list_item<tdata>*   p_next;

      p_data = start;
      while( p_data != NULL )
      {
        p_next = p_data->next;
        delete( p_data );
        p_data = p_next;
      }
      start = end = NULL;
      _size = 0;
    }
};
#endif /*__p2list_H__*/