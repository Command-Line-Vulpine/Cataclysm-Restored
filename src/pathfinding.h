#pragma once
#ifndef CATA_SRC_PATHFINDING_H
#define CATA_SRC_PATHFINDING_H

#include <optional>

#include "coordinates.h"
#include "game_constants.h"
#include "mdarray.h"
#include "character.h"

enum pf_special : int {
    PF_NORMAL = 0x00,    // Plain boring tile (grass, dirt, floor etc.)
    PF_SLOW = 0x01,      // Tile with move cost >2
    PF_WALL = 0x02,      // Unpassable ter/furn/vehicle
    PF_VEHICLE = 0x04,   // Any vehicle tile (passable or not)
    PF_FIELD = 0x08,     // Dangerous field
    PF_TRAP = 0x10,      // Dangerous trap
    PF_UPDOWN = 0x20,    // Stairs, ramp etc.
    PF_CLIMBABLE = 0x40, // 0 move cost but can be climbed on examine
    PF_SHARP = 0x80,     // sharp items (barbed wire, etc)
    PF_SMALL_PASSAGE = 0x100,   // Only medium or smaller characters can fit
};

constexpr pf_special operator | ( pf_special lhs, pf_special rhs )
{
    return static_cast<pf_special>( static_cast< int >( lhs ) | static_cast< int >( rhs ) );
}

constexpr pf_special operator & ( pf_special lhs, pf_special rhs )
{
    return static_cast<pf_special>( static_cast< int >( lhs ) & static_cast< int >( rhs ) );
}

inline pf_special &operator |= ( pf_special &lhs, pf_special rhs )
{
    lhs = static_cast<pf_special>( static_cast< int >( lhs ) | static_cast< int >( rhs ) );
    return lhs;
}

inline pf_special &operator &= ( pf_special &lhs, pf_special rhs )
{
    lhs = static_cast<pf_special>( static_cast< int >( lhs ) & static_cast< int >( rhs ) );
    return lhs;
}

struct pathfinding_cache {
    pathfinding_cache();

    bool dirty = false;
    std::unordered_set<point> dirty_points;

    cata::mdarray<pf_special, point_bub_ms> special;
};

struct pathfinding_settings {
    int bash_strength = 0;
    int max_dist = 0;
    // At least 2 times the above, usually more
    int max_length = 0;

    // Expected terrain cost (2 is flat ground) of climbing a wire fence
    // 0 means no climbing
    int climb_cost = 0;

    bool allow_open_doors = false;
    bool allow_unlock_doors = false;
    bool avoid_traps = false;
    bool allow_climb_stairs = true;
    bool avoid_rough_terrain = false;
    bool avoid_sharp = false;

    std::optional<creature_size> size = std::nullopt;

    pathfinding_settings() = default;
    pathfinding_settings( const pathfinding_settings & ) = default;

    pathfinding_settings( int bs, int md, int ml, int cc, bool aod, bool aud, bool at, bool acs,
                          bool art, bool as, std::optional<creature_size> sz = std::nullopt )
        : bash_strength( bs ), max_dist( md ), max_length( ml ), climb_cost( cc ),
          allow_open_doors( aod ), allow_unlock_doors( aud ), avoid_traps( at ), allow_climb_stairs( acs ),
          avoid_rough_terrain( art ), avoid_sharp( as ), size( sz )  {}

    pathfinding_settings &operator=( const pathfinding_settings & ) = default;
};

#endif // CATA_SRC_PATHFINDING_H
