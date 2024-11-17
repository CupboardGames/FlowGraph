// Copyright https://github.com/MothCocoon/FlowGraph/graphs/contributors

#pragma once

#include "Asset/FlowObjectDiff.h"
#include "DiffResults.h"
#include "Editor/Kismet/Private/DiffControl.h"

class FBlueprintDifferenceTreeEntry;
class SFlowDiff;
class UEdGraph;
class UEdGraphNode;
class UFlowAsset;
struct FDiffResultItem;
struct FEdGraphEditAction;

/////////////////////////////////////////////////////////////////////////////
/// FFlowAssetDiffControl
class FLOWEDITOR_API FFlowAssetDiffControl : public FDetailsDiffControl
{
public:
	FFlowAssetDiffControl(const UFlowAsset* InOldFlowAsset, const UFlowAsset* InNewFlowAsset, FOnDiffEntryFocused InSelectionCallback);

	virtual void GenerateTreeEntries(TArray<TSharedPtr<FBlueprintDifferenceTreeEntry>>& OutTreeEntries, TArray<TSharedPtr<FBlueprintDifferenceTreeEntry>>& OutRealDifferences) override;
};

/////////////////////////////////////////////////////////////////////////////
/// FFlowGraphToDiff: engine's FGraphToDiff customized to Flow Graph
struct FLOWEDITOR_API FFlowGraphToDiff : public TSharedFromThis<FFlowGraphToDiff>, IDiffControl
{
	FFlowGraphToDiff(SFlowDiff* DiffWidget, UEdGraph* GraphOld, UEdGraph* GraphNew, const FRevisionInfo& RevisionOld, const FRevisionInfo& RevisionNew);
	virtual ~FFlowGraphToDiff() override;

	/** Add widgets to the differences tree */
	virtual void GenerateTreeEntries(TArray<TSharedPtr<FBlueprintDifferenceTreeEntry>>& OutTreeEntries, TArray<TSharedPtr<FBlueprintDifferenceTreeEntry>>& OutRealDifferences) override;

	UEdGraph* GetGraphOld() const { return GraphOld; };
	UEdGraph* GetGraphNew() const { return GraphNew; };

	ENodeDiffType GetNodeDiffType(const UEdGraphNode& Node) const;

	TSharedPtr<FFlowObjectDiff> GetFlowObjectDiff(const FDiffResultItem& DiffResultItem);

	/** Source for list view */
	TArray<TSharedPtr<FDiffResultItem>> DiffListSource;
	TSharedPtr<TArray<FDiffSingleResult>> FoundDiffs;

	/** Index of the first item in RealDifferences that was generated by this graph */
	int32 RealDifferencesStartIndex = INDEX_NONE;

private:
	FText GetToolTip() const;
	TSharedRef<SWidget> GenerateCategoryWidget() const;

	/** Called when the Newer Graph is modified*/
	void OnGraphChanged(const FEdGraphEditAction& Action) const;

	void BuildDiffSourceArray();

	TSharedPtr<FFlowObjectDiff> GenerateFlowObjectDiff(const TSharedPtr<FDiffResultItem>& Differences);

	TSharedPtr<FFlowObjectDiff> FindParentNode(class UFlowGraphNode* Node);

	TMap<FString, TSharedPtr<FFlowObjectDiff>> FlowObjectDiffsByNodeName;

	SFlowDiff* DiffWidget;
	UEdGraph* GraphOld;
	UEdGraph* GraphNew;

	/** Description of Old and new graph */
	FRevisionInfo RevisionOld;
	FRevisionInfo RevisionNew;

	FDelegateHandle OnGraphChangedDelegateHandle;

};
