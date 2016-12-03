#pragma once
/***************************************************************************************************
intrusivelist.h

Simple intrusive doubly-linked list class
 
by David Ramos
***************************************************************************************************/
template <typename T>
struct tIntrusiveListNode
{
	T* prev = nullptr;
	T* next = nullptr;
};

template <typename T>
class cIntrusiveList
{
public:

};