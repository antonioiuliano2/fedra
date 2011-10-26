//-- Author :  Valeri Tioukov   31/08/2011 (modified code found in network)
#include "EdbCombGen.h"
ClassImp(EdbCombGen)

//---------------------------------------------------------------------------------
EdbCombGen::EdbCombGen( TObjArray &elements, // Original items
                        int nitems) :    // Number of items to choose
              eElements(elements), eNext(true)
{
            eSize = elements.GetEntries();
            if ( eSize > 0 )
            {
                // If no argument given, choose all items
                if ( nitems <= 0 )       eItems = eSize;
                else                     eItems = min(nitems, eSize);

                // Bit vector that controls permutations
                eBits.resize( eSize, false );

                // Initialize nItem bits in vector to "true"
                for ( int j = eSize-eItems; j < eSize; j++ )      eBits[j] = true;
            }
            else
                eNext = false; // size is 0, so no combinations are possible
}
  
//---------------------------------------------------------------------------------
bool EdbCombGen::NextCombination(TObjArray &selected, TObjArray &rejected)
{
            selected.Clear();
            rejected.Clear();

            if ( !eNext )                return false;

            int nfound = 1;
            for ( int j = 0; j < eSize; j++ )
            {
               if (eBits[j] == true )
               {
                   selected.Add( eElements[j] );
                   nfound++;
               } else 
                   rejected.Add(eElements[j]);
            }
            eNext = next_permutation(eBits.begin(), eBits.end());
            return true;
}
