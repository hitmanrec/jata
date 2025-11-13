<template>
	<div class="funny-rating">
		<div class="funny-rating__stars">
			<span 
				:class="{'funny-rating__star--active': index === rating}"
				@click="rating = ratingSymbols.indexOf(star)"
				class="funny-rating__star" v-for="(star, index) in ratingSymbols" :key="star">
				{{ star }}
			</span>
		</div>
	</div>
</template>

<script>
export default {
	name: 'funny-rating',
	props: {
		currentValue: {
			type: Number,
			default: 0
		}
	},
	data() {
		return {
			rating: this.currentValue,
			ratingSymbols: ['💩', '🤮', '😟','🤔', '😑', '😊', '🤩', '😍', '❤️', '🥵']
		}
	},
	watch: {
		currentValue(newVal) {
			this.rating = newVal
		},
		rating(newVal) {
			this.$emit('update:currentValue', newVal)
		}
	}
};
</script>

<style scoped>
.funny-rating{
	height: 20px;
	align-items: center;
}

.funny-rating__stars{
	height: 100%;
	display: flex;
	flex-direction: row;
	align-items: center;
	justify-content: center;

}

.funny-rating__star{
	cursor: pointer;
	opacity: 0.5;
	font-size: 0.9em;
	transition: opacity 0.2s ease, font-size 0.2s ease;
}

.funny-rating__star--active{
	opacity: 1;
	font-size: 1.1em;
}

.funny-rating__star:hover{
	opacity: 1;
}
</style>