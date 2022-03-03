// Fill out your copyright notice in the Description page of Project Settings.


#include "GOAPNode.h"

GOAPNode::GOAPNode(GOAPNode* InParent, float InCost, TMap<FString, int32> InStates, UGOAPTaskComponent* InTask) {
	Parent = InParent;
	Cost = InCost;
	States = InStates;
	Task = InTask;
}

GOAPNode::GOAPNode(GOAPNode* InParent, float InCost, const TMap<FString, int32>& AllStates,
                   const TMap<FString, int32>& InStates, UGOAPTaskComponent* InTask) {
	Parent = InParent;
	Cost = InCost;
	States = TMap<FString, int32>(AllStates);

	for (TPair<FString, int32> State : InStates) {
		if (!States.Contains(State.Key)) {
			States.Add(State);
		}
	}

	Task = InTask;
}

GOAPNode::~GOAPNode() {
}
