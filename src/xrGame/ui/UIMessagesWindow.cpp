// File:		UIMessagesWindow.h
// Description:	Window with MP chat and Game Log ( with PDA messages in single and Kill Messages in MP)
// Created:		22.04.2005
// Author:		Serge Vynnychenko
// Mail:		narrator@gsc-game.kiev.ua
//
// Copyright 2005 GSC Game World

#include "StdAfx.h"
#include "UIMessagesWindow.h"
#include "UIGameLog.h"
#include "UIChatWnd.h"
#include "xrUICore/XML/xrUIXmlParser.h"
#include "UIXmlInit.h"
#include "UIInventoryUtilities.h"
#include "game_news.h"
#include "UIPdaMsgListItem.h"
#include "xrGame/game_type.h"

constexpr cpcstr CHAT_LOG_LIST_PENDING = "chat_log_list_pending";

CUIMessagesWindow::CUIMessagesWindow() : CUIWindow("CUIMessagesWindow")
{
    Init(0, 0, UI_BASE_WIDTH, UI_BASE_HEIGHT);
}

void CUIMessagesWindow::AddLogMessage(KillMessageStruct& msg) const { m_pGameLog->AddLogMessage(msg); }
void CUIMessagesWindow::AddLogMessage(const shared_str& msg) const { m_pGameLog->AddLogMessage(msg.c_str()); }

void CUIMessagesWindow::PendingMode(bool const is_pending_mode)
{
    if (is_pending_mode)
    {
        if (m_in_pending_mode)
            return;

        m_pChatWnd->PendingMode(is_pending_mode);
        m_pChatLog->SetWndRect(m_pending_chat_log_rect);
        m_in_pending_mode = true;
        return;
    }
    if (!m_in_pending_mode)
        return;

    m_pChatWnd->PendingMode(is_pending_mode);
    m_pChatLog->SetWndRect(m_inprogress_chat_log_rect);
    m_in_pending_mode = false;
}

void CUIMessagesWindow::Init(float x, float y, float width, float height)
{
    CUIXml xml;
    xml.Load(CONFIG_PATH, UI_PATH, UI_PATH_DEFAULT, "messages_window.xml");
    m_pGameLog = xr_new<CUIGameLog>();
    m_pGameLog->SetAutoDelete(true);
    m_pGameLog->Show(true);
    AttachChild(m_pGameLog);
    CUIXmlInit::InitScrollView(xml, "sp_log_list", 0, m_pGameLog);
}

void CUIMessagesWindow::AddIconedPdaMessage(GAME_NEWS_DATA* news)
{
    CUIPdaMsgListItem* pItem = m_pGameLog->AddPdaMessage();

    cpcstr time_str = GetTimeAsString(news->receive_time, InventoryUtilities::etpTimeToMinutes).c_str();
    pItem->UITimeText.SetText(time_str);
    pItem->UITimeText.AdjustWidthToText();
    Fvector2 p = pItem->UICaptionText.GetWndPos();
    p.x = pItem->UITimeText.GetWndPos().x + pItem->UITimeText.GetWidth() + 3.0f;
    pItem->UICaptionText.SetWndPos(p);
    pItem->UICaptionText.SetTextST(news->news_caption.c_str());
    pItem->UIMsgText.SetTextST(news->news_text.c_str());
    pItem->UIMsgText.AdjustHeightToText();

    pItem->SetColorAnimation("ui_main_msgs_short", LA_ONLYALPHA | LA_TEXTCOLOR | LA_TEXTURECOLOR, float(news->show_time));
    pItem->UIIcon.InitTexture(news->texture_name.c_str());

    const float h1 = _max(pItem->UIIcon.GetHeight(), pItem->UIMsgText.GetWndPos().y + pItem->UIMsgText.GetHeight());
    pItem->SetHeight(h1 + 3.0f);

    m_pGameLog->SendMessage(pItem, CHILD_CHANGED_SIZE);
}

void CUIMessagesWindow::AddChatMessage(const shared_str& msg, const shared_str& author) const
{
    m_pChatLog->AddChatMessage(msg.c_str(), author.c_str());
}

void CUIMessagesWindow::Show(bool show)
{
    if (m_pChatWnd)
        m_pChatWnd->Show(show);
    if (m_pGameLog)
        m_pGameLog->Show(show);
    if (m_pChatLog)
        m_pChatLog->Show(show);
}
