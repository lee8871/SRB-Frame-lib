#include "Node.h"
#include "DumotorNode.h"
#include "MappingCluster.h"
#include <string.h>
#include "MotorAdjCluster.h"
#include "MotorSetCluster.h"

using namespace lee8871_support;
namespace srb {
	MotorSetCluster * DumotorNode::SetCLU()
	{
		return setCLU;
	}
	MotorAdjCluster * DumotorNode::AdjCLU()
	{
		return adjCLU;
	}
	Du_Motor::sDataRs * DumotorNode::Data()
	{
		return  (Du_Motor::sDataRs*const)(_base_node->data_rs);
	}
	DumotorNode::DumotorNode(Node * _base_node) :
		iExpandNode(_base_node)
	{
		setCLU = new MotorSetCluster(_base_node);
		adjCLU = new MotorAdjCluster(_base_node);
		addCluster(setCLU);
		addCluster(adjCLU);
		_base_node->setMapping(Mapping0CLU()->Data()->u8, 0);
		_base_node->setMapping(Du_Motor::Mapping1, 1);
		_base_node->setMapping(Du_Motor::Mapping2, 2);
		_base_node->setMapping(Du_Motor::Mapping3, 1);

	}
	DumotorNode::~DumotorNode() {

	}

	DumotorNode * DumotorNode::expand(iExpandNode * n) {
		if (n == nullptr) {
			return nullptr;
		}
		if (strcmp(n->Node_type(), Node_type) == 0) {
			return (DumotorNode *)n;
		}
		else {
			return nullptr;
		}
	}

	const char DumotorNode::Node_type[] = "Du_Motor";


	Json MotorAdjCluster::to_json{
#define relTo(value) (((Du_Motor::csMotorAdj*)((MotorAdjCluster*)0)->_data_u8)->value)
		{"Adj",&relTo(Adj)},
		{"TogMotorA",&relTo(TogMotorA)},
		{"TogMotorB",&relTo(TogMotorB)}
	};

	Json MotorSetCluster::to_json{
#define relTo(value) (((Du_Motor::csMotorSet*)((MotorSetCluster*)0)->_data_u8)->value)
		{"min_pwm_a",&relTo(min_pwm_a)},
		{"min_pwm_b",&relTo(min_pwm_b)},
		{"period",&relTo(period)},
		{"lose_control_ms",&relTo(lose_control_ms)},
		{"lose_behavior",&relTo(lose_behavior)}
	};


	lee8871_support::Json* DumotorNode::to_json = nullptr;
#define relTo(value) (((DumotorNode*)0)->value)
	Json* DumotorNode::ToJson() {
		if (to_json == nullptr) {
			to_json = new Json{
				{"MotorAdj",buildJsonPtr(MotorAdjCluster::to_json, &relTo(adjCLU))},
				{"MotorSet",buildJsonPtr(MotorSetCluster::to_json, &relTo(setCLU))}
			};
		}
		return to_json;
	}



}
