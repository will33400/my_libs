/*!
 * \file obj_hmi.c
 * \brief object oriented human machine interface functions
 * \author Adam LUCAS
 * \date 23/03/2022
 */

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "obj_hmi.h"

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: File Scope or Global Data                                         */
/* ************************************************************************** */
/* ************************************************************************** */


// *****************************************************************************
// *****************************************************************************
// Section: Interface Functions
// *****************************************************************************
// *****************************************************************************

//*****************************************************************************/
/*!
* \fn static void obj_hmi_default_cb(void *handle);
* \brief object oriented hmi default callback for objects that do not use any callback
* \param[inout] handle user defined callback data (not used)
* \return None
*/

static void obj_hmi_default_cb(void *handle) {
    (void)handle;
}

//*****************************************************************************/
/*!
* \fn void obj_hmi_init(obj_hmi_ctx *ctx);
* \brief object oriented hmi init routine
* \param[in] ctx user defined object data
* \return None
*/

void obj_hmi_init(obj_hmi_ctx *ctx) {

    if(ctx->init == NULL) ctx->init = obj_hmi_default_cb;
    if(ctx->compute == NULL) ctx->compute = obj_hmi_default_cb;
    if(ctx->idle == NULL) ctx->idle = obj_hmi_default_cb;
    if(ctx->pause == NULL) ctx->pause = obj_hmi_default_cb;

    ctx->active = false;
}

//*****************************************************************************/
/*!
* \fn void obj_hmi_set_active(obj_hmi_ctx *ctx_list, uint16_t index);
* \brief object oriented hmi init routine
* \param[in] ctx_list user defined object data (first object from list)
* \param[in] index object index through "next" variable addresses
* \return None
*/

void obj_hmi_set_active(obj_hmi_ctx *ctx_list, uint16_t index) {
    obj_hmi_ctx *temp = ctx_list;
    uint16_t i = 0;
    while(temp) {
        if(i != index) temp->active = false;
        else temp->active = true;
        temp = temp->next;
        i++;
    }
    
}

//*****************************************************************************/
/*!
* \fn void obj_hmi_set_state(obj_hmi_ctx *ctx, obj_hmi_state state);
* \brief object oriented hmi set object state routine
* \param[in] ctx user defined object data
* \param[in] state object state
* \return None
*/

void obj_hmi_set_state(obj_hmi_ctx *ctx, obj_hmi_state state) {
    ctx->state = state;
}

//*****************************************************************************/
/*!
* \fn void obj_hmi_enable(obj_hmi_ctx *ctx, bool enable);
* \brief object oriented hmi enable object routine
* \param[in] ctx user defined object data
* \param[in] enable object state
* \return None
*/

void obj_hmi_enable(obj_hmi_ctx *ctx, bool enable) {
    ctx->active = enable;
}

//*****************************************************************************/
/*!
* \fn void obj_hmi_process(obj_hmi_ctx *ctx_list);
* \brief object oriented hmi main process routine
* \param[in] ctx_list user defined object data (first object from list)
* \return None
*/

void obj_hmi_process(obj_hmi_ctx *ctx_list) {

    while(ctx_list) {
        if(!ctx_list->active) {
            ctx_list = ctx_list->next;
            continue;
        }

        switch (ctx_list->state)
        {
        case OBJ_HMI_STATE_INIT:
            ctx_list->init(ctx_list->handle);
        break;
        case OBJ_HMI_STATE_COMPUTE:
            ctx_list->compute(ctx_list->handle);
        break;
        case OBJ_HMI_STATE_IDLE:
            ctx_list->idle(ctx_list->handle);
        break;
        case OBJ_HMI_STATE_PAUSE:
            ctx_list->pause(ctx_list->handle);
        break;
        }

        ctx_list = ctx_list->next;
    }
}

//*****************************************************************************/
/*!
* \fn bool obj_hmi_is_in_pause(obj_hmi_ctx *ctx);
* \brief object oriented hmi get object current state pause condition routine
* \param[in] ctx user defined object data
* \return object current state is in pause (true) or not (false)
*/

bool obj_hmi_is_in_pause(obj_hmi_ctx *ctx) {
    return (ctx->state == OBJ_HMI_STATE_PAUSE) ? true : false;
}

//*****************************************************************************/
/*!
* \fn char *obj_hmi_get_state_str(obj_hmi_ctx *ctx);
* \brief object oriented hmi get string name of current state of any object routine
* \param[in] ctx user defined object data
* \return object current state name in string
*/

char *obj_hmi_get_state_str(obj_hmi_ctx *ctx) {
    return (
        ctx->state == OBJ_HMI_STATE_INIT ? "INIT" :
        ctx->state == OBJ_HMI_STATE_COMPUTE ? "COMPUTE" :
        ctx->state == OBJ_HMI_STATE_IDLE ? "IDLE" :
        ctx->state == OBJ_HMI_STATE_PAUSE ? "PAUSE" :
        "NONE"
    );
}