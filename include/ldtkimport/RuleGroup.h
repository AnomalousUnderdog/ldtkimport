#ifndef LDTK_IMPORT_RULE_GROUP_H
#define LDTK_IMPORT_RULE_GROUP_H

#include <string>
#include <vector>

#include "ldtkimport/Rule.h"


namespace ldtkimport
{

/**
 *  @brief A RuleGroup is basically like a folder for Rules.
 *  This groups together Rules for organization and ease-of-use.
 *
 *  @see https://ldtk.io/json/#ldtk-LayerDefJson;autoRuleGroups
 */
struct RuleGroup
{
   RuleGroup() :
      name(),
      active(true),
      rules()
   {
   }

   /**
    *  @brief User-defined name of the rule group.
    */
   std::string name;

   /**
    *  @brief Whether this rule active or not.
    *  Deactivated rules should be skipped over and not be processed.
    */
   bool active;

   /**
    *  @brief The actual rules in this group.
    */
   std::vector<Rule> rules;
};

} // namespace ldtkimport

#endif // LDTK_IMPORT_RULE_GROUP_H
