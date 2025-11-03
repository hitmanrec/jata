<template>
	<button class="category-button" @click="$emit('click')">
		<slot></slot>
	</button>
</template>

<script>
export default {
	name: 'category-button',
	props: {
		category: {
			type: Object,
			required: true
		}
	}
};
</script>

<style scoped>

</style>