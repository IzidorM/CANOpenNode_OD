#include <stdint.h>
#include <stdlib.h>
#include "canopen_od_interface.h"

CON_OD_ELEMENT *CO_OD_find(CON_OD *n,
                           uint16_t const index)
{
        while (n != NULL)
        {
                if (index == n->head.index)
                {
                        return n;
                }
                n = n->head.next_od_element;
        }
        return NULL;
}


static uint16_t con_get_subIndex_data(
        struct con_od_data const *d)
{
        if (NULL == d)
        {
                return 0; //add error
        }
        
        if ((OD_UINT8 == d->element_code) ||
            (OD_INT8 == d->element_code))
        {
                return 1;
        }
        else if ((OD_UINT16 == d->element_code) ||
                 (OD_INT16 == d->element_code))
        {
                return 2;
        }
        else if ((OD_UINT32 == d->element_code) ||
                 (OD_INT32 == d->element_code))
        {
                return 4;
        }
        else
        {
                return 0; // add error
        }
}

uint16_t CO_OD_getLength(CON_OD_ELEMENT const *t, uint8_t const sub_index)
{
        // TODO: handle data type domain
        uint16_t size = 0U;
        if (NULL == t)
        {
                size = 0U; //error
        }
        // array type
        else if (OD_ARRAY == t->data.element_code)
        {
                if (0U == sub_index)
                {
                        size = 1U;
                }
                else
                {
                        struct con_od_array *a = t->data.element_data;
                        if (NULL == a)
                        {
                                return 0;
                        }
                        if (sub_index > a->number_of_elements)
                        {
                                return 0;
                        }
                        
                        size = con_get_subIndex_data(&a->data);
                }
        }
        else if (OD_RECORD == t->data.element_code)
        {
                struct con_od_record *ct = t->data.element_data;
                struct con_od_record_subelement *se = ct->first_subelement;
                if ((NULL == ct) || (NULL == se))
                {
                        size = 0U;
                }
                else if (0U == sub_index) // length of first element is always 1
                {
                        size = 1U;
                        
                }
                else
                {
                        // sub_index n means we need to get n-th element
                        // from the linked list
                        if (ct->number_of_elements >= sub_index)
                        {
                                uint32_t i;
                                // start from 1, because 0-th element is the
                                // record element count and is not part of the
                                // linked list
                                for (i = 1U;  sub_index > i; i++)
                                {
                                        if (NULL == se)
                                        {
                                                break;
                                        }
                                        se = se->next_subelement;
                                }
                        }
                        // index out of range
                        else
                        {
                                se = NULL;
                        }
                        // if se is NULL, no element was found
                        if (NULL != se)
                        {
                                size =  con_get_subIndex_data(&se->data);
                        }
                        else
                        {
                                size = 0U;
                        }
                }
        }
        else
        {
                // simple data type
                size =  con_get_subIndex_data(&t->data);
        }

        return size;
}

/**
 * Get pointer to data of the given object with specific subIndex.
 *
 * If Object Type is array and subIndex is zero, function returns pointer to
 * object->maxSubIndex variable.
 *
 * @param SDO This object.
 * @param entryNo Sequence number of OD entry as returned from CO_OD_find().
 * @param subIndex Sub-index of the object in Object dictionary.
 *
 * @return Pointer to the variable in @ref CO_SDO_objectDictionary.
 */
void *CO_OD_getDataPointer(CON_OD_ELEMENT const *t, uint8_t const sub_index)
{

        if (NULL == t)
        {
                return NULL;
        }
        // array type
        else if (OD_ARRAY == t->data.element_code)
        {
                if (0U == sub_index)
                {
                        return NULL; //what to return here?
                }
                else
                {
                        struct con_od_array *a = t->data.element_data;
                        if (NULL == a)
                        {
                                return NULL;
                        }
                        if (sub_index > a->number_of_elements)
                        {
                                return NULL;
                        }

                        uint16_t element_size = CO_OD_getLength(t, sub_index);
                        return ((uint8_t *) a->data.element_data) + (element_size * (sub_index - 1));
                }
        }
        else if (OD_RECORD == t->data.element_code)
        {
                struct con_od_record *ct = t->data.element_data;
                struct con_od_record_subelement *se = ct->first_subelement;
                if ((NULL == ct) || (NULL == se))
                {
                        return NULL;
                }

                // length of first element is always 1
                if (0U == sub_index)
                {
                        return NULL; //what to return here?
                }
                else
                {
                        // sub_index n means we need to get n-th element
                        // from the linked list
                        if (ct->number_of_elements >= sub_index)
                        {
                                uint32_t i;
                                // start from 1, because 0-th element is the
                                // record element count and is not part of the
                                // linked list
                                for (i = 1U;  sub_index > i; i++)
                                {
                                        if (NULL == se)
                                        {
                                                break;
                                        }
                                        se = se->next_subelement;
                                }
                        }
                        // index out of range
                        else
                        {
                                se = NULL;
                        }
                        // if se is NULL, no element was found
                        if (NULL != se)
                        {
                                return se->data.element_data;
                        }
                        else
                        {
                                return NULL;
                        }
                }
        }
        else
        {
                // simple data type
                return (t->data.element_data);
        }
}

uint16_t CO_OD_getAttribute(CON_OD_ELEMENT const *t, uint8_t const sub_index)
{
        return t->data.attributes;

//    const CO_OD_entry_t* object = &SDO->OD[entryNo];
//
//    if(entryNo == 0xFFFFU){
//        return 0U;
//    }
//
//    if(object->maxSubIndex == 0U){   /* Object type is Var */
//        return object->attribute;
//    }
//    else if(object->attribute != 0U){/* Object type is Array */
//        uint16_t attr = object->attribute;
//        if(subIndex == 0U){
//            /* First subIndex is readonly */
//            attr &= ~(CO_ODA_WRITEABLE | CO_ODA_RPDO_MAPABLE);
//            attr |= CO_ODA_READABLE;
//        }
//        return attr;
//    }
//    else{                            /* Object type is Record */
//        return ((const CO_OD_entryRecord_t*)(object->pData))[subIndex].attribute;
//    }
        return 0;
}

uint8_t CO_OD_getFlagsPointer(CON_OD_ELEMENT const *t)
{
//    CO_OD_extension_t* ext;
//
//    if((entryNo == 0xFFFFU) || (SDO->ODExtensions == 0)){
//        return 0;
//    }
//
//    ext = &SDO->ODExtensions[entryNo];
//
//    return &ext->flags[subIndex];
        return 0;
}

uint16_t CO_OD_getIndex(CON_OD_ELEMENT const *t)
{
        if(NULL == t)
        {
                return 0;
        }

        return t->head.index;
}

char* CO_OD_getEntryName(CON_OD_ELEMENT const *t)
{
#ifdef USE_OD_DESCRIPTION
        return t->description.entry_name;
#else
        return NULL;
#endif
}

char* CO_OD_getEntryDescription(CON_OD_ELEMENT const *t)
{
#ifdef USE_OD_DESCRIPTION
        return t->description.entry_description;
#else
        return NULL;
#endif
}

int32_t con_od_add_element_to_od(CON_OD *root,
                                       struct con_od_list_node *element)
{
        if ((NULL == root) || (NULL == element))
        {
                return -1;
        }

        while (NULL != root)
        {
                if (NULL == root->head.next_od_element)
                {
                        root->head.next_od_element = element;
                        element->head.next_od_element = NULL;
                        break;
                }
                root = root->head.next_od_element;
        }
        return 0;
}

int32_t con_od_add_element_to_record(
        struct con_od_record *root,
        struct con_od_record_subelement *element)
{
        if ((NULL == root) || (NULL == element))
        {
                return -1;
        }

        if (NULL == root->first_subelement)
        {
                root->first_subelement = element;
        }
        else
        {
                struct con_od_record_subelement *tmp;
                tmp = root->first_subelement;
                
                while (NULL != tmp)
                {
                        if (NULL == tmp->next_subelement)
                        {
                                tmp->next_subelement = element;
                                element->next_subelement = NULL;
                                root->number_of_elements += 1;
                                break;
                        }
                        tmp = tmp->next_subelement;
                }
        }
        return 0;
}
