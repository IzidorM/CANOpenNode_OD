#include <stdint.h>
#include <stdlib.h>
#include "CO_OD_interface.h"

//uint16_t CO_OD_getIndex(void *t)
//{
//        struct con_od_list_node_generic const *od_node = t;
//
//        if(NULL == od_node)
//        {
//                return 0;
//        }
//
//        return od_node->index;
//}

void *CO_OD_find(void *n, uint16_t const index)
{
        struct con_od_list_node_generic *od_node = n;
        
        while (od_node != NULL)
        {
                if (index == od_node->index)
                {
                        return od_node;
                }
                od_node = od_node->next_od_element;
        }
        return NULL;
}

static uint16_t con_get_type_size(uint8_t type)
{
        if ((OD_TYPE_INT8 == type) ||
            (OD_TYPE_UINT8 == type))
        {
                return sizeof(uint8_t);
        }
        else if ((OD_TYPE_INT16 == type) ||
                 (OD_TYPE_UINT16 == type))
        {
                return sizeof(uint16_t);
        }
        else if ((OD_TYPE_INT32 == type) ||
                 (OD_TYPE_UINT32 == type))
        {
                return sizeof(uint32_t);
        }
        else
        {
                return 0; // add error
        }
}

uint16_t CO_OD_getLength(void const *n, uint8_t const sub_index)
{
        struct con_od_list_node_generic const *od_node = n;
        
        uint16_t size = 0U;
        if (NULL == od_node)
        {
                size = 0U; //error
        }
        // array type
        else if (OD_ARRAY == od_node->element_id)
        {
                if (0U == sub_index)
                {
                        size = 1U;
                }
                else
                {
                        struct con_od_list_node_array *a = (void *) n;
                        if (NULL == a)
                        {
                                return 0;
                        }
                        else if (sub_index > a->number_of_elements)
                        {
                                return 0;
                        }

                        size =  con_get_type_size(a->array_elements_data_type);
                        //size = con_get_subIndex_data(&a->data);
                }
        }
        else if (OD_RECORD == od_node->element_id)
        {
                struct con_od_list_node_record  *rec = (void *) n;
                
                if ((NULL == rec))
                {
                        size = 0U;
                }
                else if (0U == sub_index) // length of first element is always 1
                {
                        size = 1U;
                }
                else
                {
                        if ((rec->number_of_elements) >= sub_index)
                        {
                                struct con_od_record_entry *tmp;
                                tmp = (rec->subelements + (sub_index - 1));
                                size =  con_get_type_size(tmp->element_data_type);
                        }
                        else // index out of range
                        {
                                size = 0U;
                        }
                }
        }
        else if (OD_DOMAIN == od_node->element_id)
        {
                //TODO: Try what happens if size = 0
                //size = 32; //CO_SDO_BUFFER_SIZE;
                size = 32; //CO_SDO_BUFFER_SIZE;
        }
        else
        {
                // simple data type
                struct con_od_list_node_var  *v = (void *) n;
                size =  con_get_type_size(v->element_data_type);
        }

        return size;
}

///**
// * Get pointer to data of the given object with specific subIndex.
// *
// * If Object Type is array and subIndex is zero, function returns pointer to
// * object->maxSubIndex variable.
// *
// * @param SDO This object.
// * @param entryNo Sequence number of OD entry as returned from CO_OD_find().
// * @param subIndex Sub-index of the object in Object dictionary.
// *
// * @return Pointer to the variable in @ref CO_SDO_objectDictionary.
// */
void *CO_OD_getDataPointer(void const *n, uint8_t const sub_index)
{
        struct con_od_list_node_generic const *od_node = n;
        if (NULL == od_node)
        {
                return NULL;
        }
        // array type
        else if (OD_ARRAY == od_node->element_id)
        {
                struct con_od_list_node_array *a = (void *) n;
                if (0U == sub_index)
                {
                        //return NULL;
                        return &a->number_of_elements;
                }
                else
                {
                        if (NULL == a)
                        {
                                return NULL;
                        }
                        else if (sub_index > a->number_of_elements)
                        {
                                return NULL;
                        }

                        uint16_t size =  con_get_type_size(a->array_elements_data_type);
                        return ((uint8_t *) a->data) + (size * (sub_index - 1));
                }
        }
        else if (OD_RECORD == od_node->element_id)
        {

                struct con_od_list_node_record  *rec = (void *) n;
                
                if ((NULL == rec))
                {
                        return NULL;
                }
                else if (0U == sub_index) // length of first element is always 1
                {
                        return &rec->number_of_elements;
                }
                else
                {
                        if ((rec->number_of_elements) >= sub_index)
                        {
                                struct con_od_record_entry *tmp;
                                tmp = (rec->subelements + (sub_index - 1));
                                return tmp->data;
                        }
                        else // index out of range
                        {
                                return NULL;
                        }
                }
        }
        else if (OD_DOMAIN == od_node->element_id)
        {
                return NULL;
        }
        else
        {
                // simple data type
                struct con_od_list_node_var *v = (void *) od_node;
                return (v->element_data);
        }
}

uint16_t CO_OD_getAttribute(void const *n, uint8_t const sub_index)
{
        struct con_od_list_node_generic const *od_node = n;
        if (NULL == od_node)
        {
                return 0U;
        }
        else if (OD_ARRAY == od_node->element_id)
        {
                struct con_od_list_node_array *a = (void *) n;
                if (0U == sub_index)
                {
                        return a->head.attributes;
                }
                else
                {
                        if (NULL == a)
                        {
                                return 0U;
                        }
                        else if (sub_index > a->number_of_elements)
                        {
                                return 0U;
                        }

                        return a->array_elements_attributes;
                }
        }
        else if (OD_RECORD == od_node->element_id)
        {

                struct con_od_list_node_record  *rec = (void *) n;

                if ((NULL == rec))
                {
                        return 0U;
                }
                else if (0U == sub_index) // length of first element is always 1
                {
                        return rec->head.attributes;
                }
                else
                {
                        if ((rec->number_of_elements) >= sub_index)
                        {
                                struct con_od_record_entry *tmp;
                                tmp = (rec->subelements + (sub_index - 1));
                                return tmp->attributes;
                        }
                        else // index out of range
                        {
                                return 0U;
                        }
                }
        }
        else if (OD_DOMAIN == od_node->element_id)
        {
                struct con_od_list_node_domain *d = (void *) n;
                return d->head.attributes;
        }
        else
        {
                // simple data type
                struct con_od_list_node_var *v = (void *) od_node;
                return (v->head.attributes);
        }
}

uint8_t CO_OD_getMaxSubindex(const void* n)
{
        struct con_od_list_node_generic const *od_node = n;
        if (NULL == od_node)
        {
                return 0U;
        }
        else if (OD_ARRAY == od_node->element_id)
        {
                struct con_od_list_node_array *a = (void *) n;
                return a->number_of_elements;
        }
        else if (OD_RECORD == od_node->element_id)
        {

                struct con_od_list_node_record  *rec = (void *) n;
                return rec->number_of_elements;
        }
        else
        {
                // simple data type
                return 0;
        }
}

uint32_t (*CO_OD_getCallback(void *OD, const void* n))(void*)
{
        struct con_od_list_node_generic const *od_node = n;
        if (NULL == od_node)
        {
                return NULL;
        }
        else if (OD_VAR_WITH_CALLBACK == od_node->element_id)
        {
                struct con_od_list_node_var_with_callback *v = (void *) od_node;
                return v->od_callback;
        }
        else if (OD_DOMAIN == od_node->element_id)
        {
                struct con_od_list_node_domain *d = (void *) od_node;
                return d->od_callback;
        }
        else
        {
                // Callback is not supported
                return NULL;
        }
}

char* CO_OD_getEntryName(void *n)
{
        struct con_od_list_node_generic const *od_node = n;
        if (NULL == od_node)
        {
                return NULL;
        }
        
#ifdef USE_OD_DESCRIPTION
        if (OD_VAR == od_node->element_id)
        {
                struct con_od_list_node_var *v = n;
                return v->description.entry_name;
        }
        else if (OD_ARRAY == od_node->element_id)
        {
                struct con_od_list_node_array *a = n;
                return a->description.entry_name;
        }
        else if (OD_RECORD == od_node->element_id)
        {
                struct con_od_list_node_record *rec = n;
                return rec->description.entry_name;
        }
        else
        {
                return NULL;
        }
        
#else
        return NULL;
#endif
}

char* CO_OD_getEntrySubindexName(void *n, uint16_t sub_index)
{
        struct con_od_list_node_generic const *od_node = n;
        if (NULL == od_node)
        {
                return NULL;
        }
        
#ifdef USE_OD_DESCRIPTION
        if (OD_RECORD == od_node->element_id)
        {
                struct con_od_list_node_record  *rec = (void *) n;
                
                if ((NULL == rec))
                {
                        return NULL;
                }
                else if (0U == sub_index) // length of first element is always 1
                {
                        return NULL;
                }
                else
                {
                        if ((rec->number_of_elements) >= sub_index)
                        {
                                struct con_od_record_entry *tmp;
                                tmp = (rec->subelements + (sub_index - 1));
                                return tmp->description.entry_name;
                        }
                        else // index out of range
                        {
                                return NULL;
                        }
                }
        }
//        else if (OD_ARRAY == od_node->element_id)
//        {
        // TODO: there is no description name for every subindex, shoud we implement it?
//        }
//        else
        {
                return NULL;
        }
        
#else
        return NULL;
#endif
}


char* CO_OD_getEntryDescription(void *n)
{
        struct con_od_list_node_generic const *od_node = n;
        if (NULL == od_node)
        {
                return NULL;
        }
        
#ifdef USE_OD_DESCRIPTION
        if (OD_VAR == od_node->element_id)
        {
                struct con_od_list_node_var *v = n;
                return v->description.entry_description;
        }
        else if (OD_ARRAY == od_node->element_id)
        {
                struct con_od_list_node_array *a = n;
                return a->description.entry_description;
        }
        else if (OD_RECORD == od_node->element_id)
        {
                struct con_od_list_node_record *rec = n;
                return rec->description.entry_description;
        }
        else
        {
                return NULL;
        }
        
#else
        return NULL;
#endif
}

char* CO_OD_getEntrySubindexDescription(void *n, uint16_t sub_index)
{
        struct con_od_list_node_generic const *od_node = n;
        if (NULL == od_node)
        {
                return NULL;
        }
        
#ifdef USE_OD_DESCRIPTION
        if (OD_RECORD == od_node->element_id)
        {
                struct con_od_list_node_record  *rec = (void *) n;
                
                if ((NULL == rec))
                {
                        return NULL;
                }
                else if (0U == sub_index) // length of first element is always 1
                {
                        return NULL;
                }
                else
                {
                        if ((rec->number_of_elements) >= sub_index)
                        {
                                struct con_od_record_entry *tmp;
                                tmp = (rec->subelements + (sub_index - 1));
                                return tmp->description.entry_description;
                        }
                        else // index out of range
                        {
                                return NULL;
                        }
                }
        }
//        else if (OD_ARRAY == od_node->element_id)
//        {
        // TODO: there is no description for every subindex, shoud we implement it?
//        }
//        else
        {
                return NULL;
        }
        
#else
        return NULL;
#endif
}



uint8_t CO_OD_getObjectType(void *n)
{
        struct con_od_list_node_generic const *od_node = n;
        if (OD_ARRAY == od_node->element_id)
        {
                return OD_OBJECT_TYPE_ARRAY;
        }
        else if (OD_RECORD == od_node->element_id)
        {
                return OD_OBJECT_TYPE_RECORD;
        }
        else if (OD_VAR == od_node->element_id)
        {
                return OD_OBJECT_TYPE_VAR;
        }
        else
        {
                return OD_OBJECT_TYPE_NULL;
        }
        
}

uint16_t CO_OD_getDataType(void *n, uint16_t sub_index)
{
        struct con_od_list_node_generic const *od_node = n;
        if (NULL == od_node)
        {
                return 0U;
        }
        else if (OD_ARRAY == od_node->element_id)
        {
                struct con_od_list_node_array *a = (void *) n;
                return a->array_elements_data_type;
        }
        else if (OD_RECORD == od_node->element_id)
        {

                struct con_od_list_node_record  *rec = (void *) n;

                if ((NULL == rec))
                {
                        return 0U;
                }
                else if (0U == sub_index) // data type of first element is always uint8
                {
                        return OD_TYPE_UINT8; 
                }
                else
                {
                        if ((rec->number_of_elements) >= sub_index)
                        {
                                struct con_od_record_entry *tmp;
                                tmp = (rec->subelements + (sub_index - 1));
                                return tmp->element_data_type;
                        }
                        else // index out of range
                        {
                                return 0U;
                        }
                }
        }
        else
        {
                // simple data type
                struct con_od_list_node_var *v = (void *) od_node;
                return (v->element_data_type);
        }
}

int32_t con_od_add_element_to_od(void *root, void * new_od_element)
{
        struct con_od_list_node_generic *root_head = root;
        struct con_od_list_node_generic *new_element_head = new_od_element;
        
        if ((NULL == root_head) || (NULL == new_od_element))
        {
                return -1;
        }

        while (NULL != root_head)
        {
                if (NULL == root_head->next_od_element)
                {
                        root_head->next_od_element = new_od_element;
                        new_element_head->next_od_element = NULL;
                        break;
                }
                root_head = root_head->next_od_element;
        }
        return 0;
}

