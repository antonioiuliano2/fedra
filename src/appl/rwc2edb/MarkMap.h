#ifndef _SYSAL_MARK_MAP_
#define _SYSAL_MARK_MAP_

#define MARK_STATUS_NOTSEARCHED		0
#define MARK_STATUS_FOUND			1
#define MARK_STATUS_NOTFOUND		-1

#pragma pack(push)
#pragma pack(1)

typedef struct 
{
	unsigned Id;
	struct t_Pos
	{
		float X, Y;
		} Nominal, Stage;
	int Status;
	} Mark;

typedef struct 
{
	unsigned Count;
	Mark *pMarks;
	} MarkMap;

#pragma pack(pop)

#endif