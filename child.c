
#include <stdio.h>
#include <math.h>
#include "pvm3.h"
#define width 10
#define height 10

int main()
{
    int masterid, row_id, row1[width], row2[width], result[width], i;
    masterid = pvm_parent();
    if (masterid == 0)
        return 1;

    while (1)
    {
        pvm_recv(masterid, 100);
        pvm_upkint(&row_id, 1, 1);
        pvm_upkint(&row1[0], width, 1);
        pvm_upkint(&row2[0], width, 1);

        for (i = 0; i < width; i++)
        {
            result[i] = row1[i] * row2[i];
        }

        pvm_initsend(PvmDataDefault);
        pvm_pkint(&row_id, 1, 1);
        pvm_pkint(&result[0], width, 1);
        pvm_send(masterid, 200);
    }

    pvm_exit();
    return 0;
}
