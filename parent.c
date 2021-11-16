#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "pvm3.h"
#define height 10
#define width 10

int main()
{
    int matrix1[height][width];
    int matrix2[height][width];

    int result[height][width];

    int i, j;

    srand(time(0));
    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            matrix1[i][j] = rand() % 100;
            matrix2[i][j] = rand() % 100;
        }
    }

    int ilhost, ilarch;
    struct pvmhostinfo *info;
    pvm_config(&ilhost, &ilarch, &info);
    printf("number of hosts: %d\n", ilhost);

    int row_id = 0;
    int tid;

    for (i = 0; i < ilhost; i++)
    {
        pvm_spawn("/home/pvm/pvm3/sekcja7/bin/LINUX/child", 0, PvmTaskHost, info[i].hi_name, 1, &tid);
        if (tid < 0)
        {
            ilhost--;
            continue;
        }
        pvm_initsend(PvmDataDefault);
        pvm_pkint(&row_id, 1, 1);
        pvm_pkint(&matrix1[row_id][0], width, 1);
        pvm_pkint(&matrix2[row_id][0], width, 1);
        pvm_send(tid, 100);
        row_id++;
    }

    int bufid, child_tid, child_row_id, tmp;
    while (row_id < height)
    {
        bufid = pvm_recv(-1, 200);
        pvm_bufinfo(bufid, &tmp, &tmp, &child_tid);
        pvm_upkint(&child_row_id, 1, 1);
        pvm_upkint(&result[child_row_id][0], width, 1);
        pvm_initsend(PvmDataDefault);
        pvm_pkint(&row_id, 1, 1);
        pvm_pkint(&matrix1[row_id][0], width, 1);
        pvm_pkint(&matrix2[row_id][0], width, 1);

        pvm_send(child_tid, 100);
        row_id++;
    }

    for (i = 0; i < ilhost; i++)
    {
        bufid = pvm_recv(-1, 200);
        pvm_bufinfo(bufid, &tmp, &tmp, &child_tid);
        pvm_upkint(&child_row_id, 1, 1);
        pvm_pkint(&matrix1[child_row_id][0], width, 1);
        pvm_pkint(&matrix2[child_row_id][0], width, 1);
    }
    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            printf("%d\t", result[i][j]);
        }
        printf("\n");
    }

    pvm_exit();

    return 0;
}
