////////////////////////////////////////////////////////////////////////////
//	Module 		: object_item_client_server_inline.h
//	Created 	: 27.05.2004
//  Modified 	: 30.06.2004
//	Author		: Dmitriy Iassenev
//	Description : Object item client and server class inline functions
////////////////////////////////////////////////////////////////////////////

#ifndef object_item_client_server_inlineH
#define object_item_client_server_inlineH

#pragma once

#define TEMPLATE_SPECIALIZATION template <typename _client_type, typename _server_type>
#define CSObjectItemClientServer CObjectItemClientServer<_client_type, _server_type>

TEMPLATE_SPECIALIZATION
IC CSObjectItemClientServer::CObjectItemClientServer(const CLASS_ID& clsid, LPCSTR script_clsid)
    : inherited(clsid, script_clsid)
{
}

TEMPLATE_SPECIALIZATION
ObjectFactory::ClientObjectBaseClass* CSObjectItemClientServer::client_object() const
{
    return (xr_new<CLIENT_TYPE>())->_construct();
}

TEMPLATE_SPECIALIZATION
ObjectFactory::ServerObjectBaseClass* CSObjectItemClientServer::server_object(LPCSTR section) const
{
    ObjectFactory::ServerObjectBaseClass* o = (xr_new<SERVER_TYPE>(section))->init();
    R_ASSERT(o);
    return (o);
}

#undef TEMPLATE_SPECIALIZATION
#undef CSObjectItemClientServer

#define TEMPLATE_SPECIALIZATION                                                                     \
    template <typename _client_type_single, typename _client_type_mp, typename _server_type_single, \
        typename _server_type_mp>
#define CSObjectItemClientServerSingleMp \
    CObjectItemClientServerSingleMp<_client_type_single, _client_type_mp, _server_type_single, _server_type_mp>

TEMPLATE_SPECIALIZATION
IC CSObjectItemClientServerSingleMp::CObjectItemClientServerSingleMp(const CLASS_ID& clsid, LPCSTR script_clsid)
    : inherited(clsid, script_clsid)
{
}

TEMPLATE_SPECIALIZATION
ObjectFactory::ClientObjectBaseClass* CSObjectItemClientServerSingleMp::client_object() const
{
    ObjectFactory::ClientObjectBaseClass* result = xr_new<_client_type_single>();

    return (result->_construct());
}

TEMPLATE_SPECIALIZATION
ObjectFactory::ServerObjectBaseClass* CSObjectItemClientServerSingleMp::server_object(LPCSTR section) const
{
    ObjectFactory::ServerObjectBaseClass* result = xr_new<_server_type_single>(section);

    result = result->init();
    R_ASSERT(result);
    return (result);
}

#undef TEMPLATE_SPECIALIZATION
#undef CSObjectItemClientServerSingleMp

#endif
