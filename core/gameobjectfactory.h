#pragma once
/***************************************************************************************************
gameobjectfactory.h

Really simple game object factory class. Manages objects in pages of contiguous storage 
 
by David Ramos
***************************************************************************************************/
template <class T, size_t kPageSize>
class cGameObjectFactory : public IGameObjectFactory
{
public:
	cGameObjectFactory()
	{
		m_initialPage = new tPage;
		m_currentPage = m_initialPage;
	}

	template <typename... Args>
	T* createGameObject(Args&&... args)
	{
		if (m_currentPageObjects > kPageSize)
		{
			addNewPage();
		}

		const size_t newObjectIdx = m_currentPageObjects++;
		return new(&m_currentPage[newObjectIdx]) T(std::forward<Args>(args)...);
	}

	template <typename Func>
	void forEach(Func&& func)
	{
		for (tPage* page = m_initialPage; page != m_currentPage; page = page->m_next)
		{
			for (auto& object : page->m_storage)
			{
				func(reinterpret_cast<T&>(object));
			}
		}

		for (size_t idx = 0; idx < m_currentPageObjects; ++idx)
		{
			func(reinterpret_cast<T&>(m_currentPage->m_storage));
		}
	}

	// Erases objectToErase while maintaining contiguity of all objects
	void erase(T& objectToErase)
	{
		// TODO: Assert if &objectToErase is not part of any of the pages

		if (m_currentPageObjects == 0)
		{
			DI_assert(m_currentPage->m_prev != nullptr, "We removed one element from an empty factory? How is that possible?")

			m_currentPage = m_currentPage->m_prev;
			m_currentPageObjects = kPageSize;
		}

		--m_currentPageObjects;
		T& objectToSwap = m_currentPage->m_storage[m_currentPageObjects];

		using std::swap;
		swap(objectToErase, objectToSwap);
	}

private:
	// TODO: Implement an intrusive doubly-linked list so we can move the logic of list iteration and maintenance out of this class
	struct tPage
	{
		tPage*				m_prev;
		tPage*				m_next;
		tAlignedStorage<T>	m_storage[kPageSize];
	};

	void addNewPage()
	{
		DI_assert(m_currentPage, "Assuming the factory had at least 1 page. Did we not create the initial page already?");

		if (m_currentPage->m_next)
		{
			m_currentPage = m_currentPage->m_next;
		}
		else
		{
			m_currentPage->m_next = new tPage;
			m_currentPage->m_next->m_prev = m_currentPage;
			m_currentPage = m_currentPage->m_next;
		}

		m_currentPageObjects = 0;
	}


	size_t m_currentPageObjects	= 0;
	tPage* m_currentPage		= nullptr;
	tPage* m_initialPage		= nullptr;
};