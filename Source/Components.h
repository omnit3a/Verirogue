#ifndef COMPONENT_H_
#define COMPONENT_H_

typedef struct ho{
	int hydration;
} Hydration;

typedef struct sex{
	char sex;
} EntitySex;

typedef struct hp{
	int currentHealth;
} Health;

typedef struct pos{
	int xPos;
	int yPos;
} Position;

typedef struct bp{
	Health bpHP;
	Position currentPos;
} BodyPart;

typedef struct lbs{
	int pounds;
} Weight;

typedef struct flame{
	int flammabilityLevel;
} Flammability;

typedef struct temp{
	int celsius;
	int fahrenheit;
} Temperature;

typedef struct starsign{
	char sign;
} StarSign;

typedef struct vitalstats{
	int strength;
	int agility;
	int intelligence;
} VitalStats;

typedef struct cas{
	char caste;
} Caste;

typedef struct hoid{
	int entityID;
	Position currentPos;

	EntitySex currentSex;

	Weight currentWeight;
	Flammability currentFlammability;
	Temperature currentTemperature;
	Hydration currentHydration;
	VitalStats currentStats;
	StarSign currentSign;

	BodyPart skin;
	BodyPart head;
	BodyPart torso;
	BodyPart leftArm;
	BodyPart rightArm;
	BodyPart leftLeg;
	BodyPart rightLeg;
} Humanoid;

typedef struct ioid{
	int entityID;
	Position currentPos;

	Caste currentCaste;

	Weight currentWeight;
	Temperature currentTemperature;
	StarSign currentSign;

	BodyPart head;
	BodyPart abdomen;
	BodyPart thorax;
	BodyPart leftLegs;
	BodyPart rightLegs;

} Insectoid;

#endif
