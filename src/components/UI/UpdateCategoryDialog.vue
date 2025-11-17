<template>
	<div class="base-dialog" v-if="show" @click="closeDialog">
		<div @click.stop class="dialog-content">
			<slot :actionType="actionType" :category="category"></slot>
		</div>
	</div>
</template>

<script>
export default {
	name: 'update-category-dialog',
	props: {
		show: {
			type: Boolean,
			required: true
		},
		actionType: {
			type: String,
			required: true
		},
		category: {
			type: Object
		}
	},
	methods: {
		closeDialog() {
			this.$emit('update:show', false)
		}
	}
};
</script>

<style scoped>
.dialog-content{
	margin: auto;
	background: var(--dark-bg-col);
	border-radius: var(--small-border-rad);
	min-height: 50px;
	min-width: 300px;
	padding: 20px;
}
</style>