#include "stdafx.h"
#pragma hdrstop

#include <DirectXTex.h>

constexpr cpcstr NOT_EXISTING_TEXTURE = "ed" DELIMITER "ed_not_existing_texture";

void fix_texture_name(pstr fn)
{
    pstr _ext = strext(fn);
    if (_ext && (!xr_stricmp(_ext, ".tga") || !xr_stricmp(_ext, ".dds") || !xr_stricmp(_ext, ".bmp") || !xr_stricmp(_ext, ".ogm")))
    {
        *_ext = 0;
    }
}

int get_texture_load_lod(LPCSTR fn)
{
    CInifile::Sect& sect = pSettings->r_section("reduce_lod_texture_list");
    auto it_ = sect.Data.cbegin();
    auto it_e_ = sect.Data.cend();

    ENGINE_API bool is_enough_address_space_available();
    static bool enough_address_space_available = is_enough_address_space_available();

    auto it = it_;
    auto it_e = it_e_;

    for (; it != it_e; ++it)
    {
        if (strstr(fn, it->first.c_str()))
        {
            if (psTextureLOD < 1)
            {
                if (enough_address_space_available)
                    return 0;
                else
                    return 1;
            }
            else if (psTextureLOD < 3)
                return 1;
            else
                return 2;
        }
    }

    if (psTextureLOD < 2)
        return 0;
    else if (psTextureLOD < 4)
        return 1;
    else
        return 2;
}

u32 calc_texture_size(int lod, u32 mip_cnt, size_t orig_size)
{
    if (1 == mip_cnt)
        return orig_size;

    int _lod = lod;
    float res = float(orig_size);

    while (_lod > 0)
    {
        --_lod;
        res -= res / 1.333f;
    }
    return iFloor(res);
}

const float _BUMPHEIGH = 8.f;

//////////////////////////////////////////////////////////////////////
// Utility pack
//////////////////////////////////////////////////////////////////////
IC void Reduce(int& w, int& h, int& l, int& skip)
{
    while ((l > 1) && skip)
    {
        w /= 2;
        h /= 2;
        l -= 1;

        skip--;
    }
    if (w < 1)
        w = 1;
    if (h < 1)
        h = 1;
}

IC void Reduce(size_t& w, size_t& h, size_t& l, int skip)
{
    while ((l > 1) && skip)
    {
        w /= 2;
        h /= 2;
        l -= 1;

        skip--;
    }
    if (w < 1)
        w = 1;
    if (h < 1)
        h = 1;
}

ID3DBaseTexture* CRender::texture_load(LPCSTR fRName, u32& ret_msize)
{
    ret_msize = 0;
    R_ASSERT1_CURE(fRName && fRName[0], true, { return nullptr; });

    DirectX::TexMetadata IMG;
    DirectX::ScratchImage texture;
    ID3DBaseTexture* pTexture2D = NULL;
    string_path fn;
    size_t img_size = 0;
    int img_loaded_lod = 0;
    u32 mip_cnt = u32(-1);
    bool dummyTextureExist;

    // make file name
    string_path fname;
    xr_strcpy(fname, fRName);
    fix_texture_name(fname);
    bool force_srgb = o.linear_space_rendering && !strstr(fname, "_bump") && !strstr(fname, "_mask") && !strstr(fname, "_dudv") && !strstr(fname, "water_normal") && !strstr(fname, "internal_") && !strstr(fname, "_lm.") && !strstr(fname, "level_lods_nm") && !strstr(fname, "lmap#") && !strstr(fname, "ui_magnifier2");

    IReader* S = NULL;
    if (!FS.exist(fn, "$game_textures$", fname, ".dds") && strstr(fname, "_bump"))
        goto _BUMP_from_base;
    if (FS.exist(fn, "$level$", fname, ".dds"))
        goto _DDS;
    if (FS.exist(fn, "$game_saves$", fname, ".dds"))
        goto _DDS;
    if (FS.exist(fn, "$game_textures$", fname, ".dds"))
        goto _DDS;

#ifdef _EDITOR
    ELog.Msg(mtError, "Can't find texture '%s'", fname);
    return 0;
#else
    Msg("! Can't find texture '%s'", fname);
    dummyTextureExist = FS.exist(fn, "$game_textures$", NOT_EXISTING_TEXTURE, ".dds");
	R_ASSERT3(dummyTextureExist, "Dummy texture doesn't exist", NOT_EXISTING_TEXTURE);
    if (!dummyTextureExist)
        return nullptr;
#endif

_DDS:
{
    // Load and get header
    S = FS.r_open(fn);
    img_size = S->length();
#ifdef DEBUG
    Msg("* Loaded: %s[%zu]", fn, img_size);
#endif // DEBUG
    R_ASSERT(S);

    R_CHK2(LoadFromDDSMemory(S->pointer(), S->length(), DirectX::DDS_FLAGS_PERMISSIVE, &IMG, texture), fn);

    // Check for LMAP and compress if needed
    xr_strlwr(fn);

    img_loaded_lod = get_texture_load_lod(fn);

    size_t mip_lod = 0;
    if (img_loaded_lod && !IMG.IsCubemap())
    {
        const auto old_mipmap_cnt = IMG.mipLevels;
        Reduce(IMG.width, IMG.height, IMG.mipLevels, img_loaded_lod);
        mip_lod = old_mipmap_cnt - IMG.mipLevels;
    }

    // DirectX requires compressed texture size to be
    // a multiple of 4. Make sure to meet this requirement.
    if (DirectX::IsCompressed(IMG.format))
    {
        IMG.width = (IMG.width + 3u) & ~0x3u;
        IMG.height = (IMG.height + 3u) & ~0x3u;
    }

    R_CHK2(CreateTextureEx(HW.pDevice, texture.GetImages() + mip_lod, texture.GetImageCount(), IMG, D3D_USAGE_IMMUTABLE, D3D_BIND_SHADER_RESOURCE, 0, IMG.miscFlags, force_srgb ? DirectX::CREATETEX_FORCE_SRGB : DirectX::CREATETEX_DEFAULT, &pTexture2D), fn);
    FS.r_close(S);

    // OK
    mip_cnt = IMG.mipLevels;
    ret_msize = calc_texture_size(img_loaded_lod, mip_cnt, img_size);
    return pTexture2D;
}

_BUMP_from_base:
{
    Msg("! Fallback to default bump map: %s", fname);
    //////////////////
    if (strstr(fname, "_bump#"))
    {
        R_ASSERT2(FS.exist(fn, "$game_textures$", "ed\\ed_dummy_bump#", ".dds"), "ed_dummy_bump#");
        S = FS.r_open(fn);
        R_ASSERT2(S, fn);
        goto _DDS;
    }
    if (strstr(fname, "_bump"))
    {
        R_ASSERT2(FS.exist(fn, "$game_textures$", "ed\\ed_dummy_bump", ".dds"), "ed_dummy_bump");
        S = FS.r_open(fn);

        R_ASSERT2(S, fn);
        goto _DDS;
    }
    //////////////////
}

    return nullptr;
}
