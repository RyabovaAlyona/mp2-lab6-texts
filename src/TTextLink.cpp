#include <conio.h>
#include <string>
#include "ttextlink.h"
#include "ttext.h"

TTextMem TTextLink::MemHeader;

void TTextLink :: InitMemSystem (int size) { // ������������� ������
  MemHeader.pFirst = (PTTextLink) new char[sizeof(TTextLink)*size];
  MemHeader.pFree  = MemHeader.pFirst;
  MemHeader.pLast  = MemHeader.pFirst + (size-1);
  PTTextLink pLink = MemHeader.pFirst;
  for (int i = 0; i < size - 1; i++, pLink++ ) { // �������� ������
    pLink -> pNext = pLink + 1;
  }
  pLink -> pNext = NULL;
}

void TTextLink :: PrintFreeLink (void) { // ������ ��������� �������
  PTTextLink pLink = MemHeader.pFree;
  std::cout << "List of free links" << std::endl;
  for ( ; pLink != NULL; pLink = pLink -> pNext )
    std::cout << pLink -> Str << std::endl;
}

void * TTextLink :: operator new (size_t size) { // ��������� �����
  PTTextLink pLink = MemHeader.pFree;
  if (MemHeader.pFree != NULL) 
	  MemHeader.pFree = pLink->pNext;
  else
	  throw -1;
  return pLink;
}

void TTextLink :: operator delete (void *pM) { // ������������ �����
	PTTextLink pLink = (PTTextLink)pM;
	pLink->pNext = MemHeader.pFree;
	MemHeader.pFree = pLink;
}

void TTextLink :: MemCleaner(TText &txt) { // ������ ������
  // ������� ������ - � ������ ������ ���� �����
  std::string st;
  // ���������� ����� ������ - ������ "&&&"
  for ( txt.Reset(); !txt.IsTextEnded(); txt.GoNext() ) {
	st = txt.GetLine();
    if ( st.find("&&&") != 0 ) 
		txt.SetLine("&&&" + txt.GetLine());
  }
  // ���������� ������ ��������� �������
  PTTextLink pLink = MemHeader.pFree;
  for ( ; pLink != NULL; pLink = pLink->pNext ) 
	  strcpy(pLink->Str,"&&&");
  // ������ ������
  pLink = MemHeader.pFirst;
  for ( ; pLink <= MemHeader.pLast; pLink++ ) {
    if ( strstr(pLink->Str,"&&&") != NULL ) { // ������ ������ ��� �������� �����
      strcpy(pLink->Str, pLink->Str+3);        // ������ ����������
    }
    else 
		delete pLink; // "����������" ����� � ������ ���������
  }
}

TTextLink & TTextLink :: operator= (TTextLink &q) {
	*this = *(PTTextLink)GetCopy();
	return *this;
}

std::ostream& operator<<(std::ostream& os, const TTextLink& instance) {
	return os << " " << instance.Str;
}
